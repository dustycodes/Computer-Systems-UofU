#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <strings.h>
#include <math.h>

#include "cachelab.h"


/*
 * *******************************************************
 * Type defined for the tags to identify and 64-bit size
 * *******************************************************
 */
typedef unsigned long long int memory_address;


/*
 * *******************************************************
 * Data structure fo the cache, a cache_object has many
 * cache_set objects. A cache_set object has many set_line
 * objects. A set_line object contains data important to
 * create a functioning cache. This includes the valid bit
 * which will decide if the cache is filled already as
 * being set 1/0. The block is unused but would contain
 * memory. The used will iterate everytime that point is
 * hit in the cache so we can decide what is least used
 * when eviction needs to occur. Finally the tag is used
 * to compare entries in the cache.
 * *******************************************************
 */
typedef struct {
	int used;
	int valid;
	memory_address tag;
	char *block;
} set_line;

typedef struct {
	set_line *lines;
} cache_set;

typedef struct {
	 cache_set *sets;
} cache_object;

typedef struct {
	int s; /* used to determine sets in the cache*/
	int b; /* used to determin size of the cache */
	int E; /* number of cachelines per set */
	long long S; /* sets: S = 2^s */
	int B; /* block size: B = 2**b (bytes)*/

	cache_object cache;
	/* keep stats on the cache */
	int hit_count;
	int miss_count;
	int evict_count;
} cache_container;


/*
 * *******************************************************
 * For debugging, not required. Simply reads the cache in
 * the container.
 * *******************************************************
 */

/*void debug(cache_container cache) {
  set_obj set;
  line_obj line;
  printf("\n--------\n");
 for(int set_it = 0; set_it < container.S; set_it = set_it + 1) {
    for (int line_it = 0; line_it < container.E; line_it = line_it + 1) {
       set.lines[line_it] = line;
       printf("set %d, line %d : tag->%llx\n", set_it, line_it, cache.sets[set_it].lines[line_it].tag);
    }
  }
 printf("--------\n");
}*/


/*
 * *******************************************************
 * Creates a cache with the given number of sets and line
 * sizes. Returns the created cache to the caller.
 * *******************************************************
 */
cache_object create(long long S, int E) {
	cache_object created_cache;
	cache_set set;
	set_line line;

	/* allocation of the space required for each set*/
	created_cache.sets = (cache_set *) malloc(sizeof(cache_set) * S);

	for (int set_it = 0; set_it < S; set_it ++) {

		/* allocation of the space required for each line */
		set.lines =  (set_line *) malloc(sizeof(set_line) * E);

		/* save empty in set */
		created_cache.sets[set_it] = set;
		for (int line_it = 0; line_it < E; line_it ++)
		{
			line.used = 0;
			/* valid is 0 means empty */
			line.valid = 0;
			line.tag = 0;
			/* save empty in line */
			set.lines[line_it] = line;
		}
	}

	return created_cache;
}


/*
 * *******************************************************
 * Run a simulation on the given address. If the address
 * is contained in the cache, mark as a hit. If the address
 * is not, mark as a miss. If a miss and the cache is full,
 * mark as eviction
 * *******************************************************
 */
cache_container simulate(cache_container container, memory_address address) {
		int full = 1;
		int original_hits = container.hit_count;
		int tag_size = (64 - (container.s + container.b));
		memory_address input_tag = address >> (container.s + container.b);
		unsigned long long temp = address << (tag_size);
		unsigned long long setIndex = temp >> (tag_size + container.b);
  		cache_set temp_set = container.cache.sets[setIndex];

		for (int line_it = 0; line_it < container.E; line_it = line_it + 1) 	{

			set_line line = temp_set.lines[line_it];

			if (line.valid) {

				if (line.tag == input_tag) {

					line.used ++;
					container.hit_count = container.hit_count + 1;
					temp_set.lines[line_it] = line;
				}

			} else if (!(line.valid) && (full)) {
				//We found an empty line
				full = 0;
			}

		}

		// Testing hit or miss
		if (original_hits == container.hit_count)
			container.miss_count = container.miss_count + 1;
		else
			return container;

		int max = temp_set.lines[0].used;
		int min = temp_set.lines[0].used;
		int min_loc = 0;
		set_line line;

		for (int line_it = 1; line_it < container.E; line_it = line_it + 1) {
			line = temp_set.lines[line_it];
			if (min > line.used) {
				min_loc = line_it;
				min = line.used;
			}
			if (max < line.used)
				max = line.used;
		}

		if (!full){
			int empty = -1;
			for (int i = 0; i < container.E; i = i + 1) {
				line = temp_set.lines[i];
				if (line.valid == 0) {
					empty = i;
					break;
				}
			}

			temp_set.lines[empty].tag = input_tag;
			temp_set.lines[empty].valid = 1;
			temp_set.lines[empty].used = max + 1;
		}

		else {
			container.evict_count++;
			temp_set.lines[min_loc].tag = input_tag;
			temp_set.lines[min_loc].used = max + 1;
		}

		return container;
}

/*
 * *******************************************************
 * The main method parces the arguments:
 *  s
 *  E
 *  b
 *  t
 *  v
 *  h
 * Which specify different attributes of the cache as well
 * as the trace file input into the program.
 * *******************************************************
 */
int main(int argc, char **argv)
{
	cache_container container;
	container.hit_count = 0;
	container.miss_count = 0;
	container.evict_count = 0;
	int v_flag = 0;

	char *trace_path;
	char c;
    while( (c=getopt(argc,argv,"s:E:b:t:vh")) != -1)
	{
        switch(c)
		{
        case 's':
            container.s = atoi(optarg);
            break;
        case 'E':
            container.E = atoi(optarg);
            break;
        case 'b':
            container.b = atoi(optarg);
            break;
        case 't':
            trace_path = optarg;
            break;
        case 'v':
            v_flag = 1;
            break;
        case 'h':
            exit(0);
        default:
            exit(1);
        }
    }

    if (container.s == 0 || container.E == 0 || container.b == 0 || trace_path == NULL)
        exit(1);


	// set_count is S
	container.S = pow(2.0, container.s);

	container.cache = create(container.S, container.E);

	// Open the trace file
	FILE *trace_file;
	char command;
	memory_address address;
	int size;
	trace_file  = fopen(trace_path, "r");


	if (trace_file != NULL) {
		while (fscanf(trace_file, " %c %llx,%d", &command, &address, &size) == 3) {
			if (v_flag)
			      printf("%c %llx,%d", command, address, size);
			switch(command) {
				case 'I':
					break;
				case 'L':
					container = simulate(container, address);
					break;
				case 'S':
					container = simulate(container, address);
					break;
				case 'M':
					container = simulate(container, address);
					container = simulate(container, address);
					break;
				default:
					break;
			}
			if(v_flag)
			      printf("\n");
		}
	}

    printSummary(container.hit_count, container.miss_count, container.evict_count);

    /* Cleaning out the cache */
    for (int setIndex = 0; setIndex < container.S; setIndex ++){
		cache_set set = container.cache.sets[setIndex];
		if (set.lines != NULL)
			free(set.lines);

	}
	if (container.cache.sets != NULL) {
		free(container.cache.sets);
	}

	/* Closing the file */
	fclose(trace_file);

    return 0;
}
