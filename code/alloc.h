#pragma once
#include<memory>
#include<cstdio>
namespace PWL
{
	union Memory_List
	{
		union Memory_List * next; // point to next memory node
		char data[1]; //current data address
	};
	enum 
	{
		Memory_List_Number = 56
	};
	enum
	{
		Align128 = 8,
		Align256 = 16,
		Align512 = 32,
		Align1024 = 64,
		Align2048 = 128,
		Align4096 = 256
	};
	enum
	{
		SmallObejectBytes = 4096
	};
	class Alloc
	{
	private:
		static char* first_memory;
		static char* last_memory;
		static size_t heap_size; // the extra memory's size in the heap
		
		static Memory_List* memory_list[Memory_List_Number];
	public:
		void* Allocate(size_t n);
		void Deallocate(void* p,size_t n);
		void* Reallocate(void* p,size_t new_value,size_t old_value);

	private:
		static size_t M_align(size_t bytes);
		static size_t M_round_up(size_t bytes);
		static size_t M_memorylist_index(size_t bytes);
		static void*  M_refill(size_t n);

		static char* M_chunk_alloc(size_t size, size_t& nblocks);
	};
	//        intial statis member
	char* Alloc::first_memory = nullptr;
	char* Alloc::last_memory = nullptr;
	size_t Alloc::heap_size = 0;

	Memory_List* Alloc::memory_list[] = {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr ,
										 nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr ,
										 nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr ,
										 nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr ,
										 nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr ,
										 nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr ,
										 nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr ,
										 nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };

	inline void* Alloc::Allocate(size_t n)
	{
		Memory_List * my_memory_list;
		Memory_List* result;
		if(n > static_cast<size_t>(SmallObejectBytes))
			return std::malloc(n); // too big ,use default malloc
		my_memory_list = memory_list[M_memorylist_index(n)];
		result = my_memory_list;
		if(result == nullptr)
		{
			void* r = M_refill(M_round_up(n));
			return r;
		}
		my_memory_list = result->next;
		return result;

	}

	inline void Alloc::Deallocate(void* p, size_t n)
	{
		if(n > static_cast<size_t>(SmallObejectBytes))
		{
			free(p);
			return;
		}
		Memory_List* q = reinterpret_cast<Memory_List*>(p);
		Memory_List* my_memory_list = memory_list[M_memorylist_index(n)];
		q->next = my_memory_list;
		my_memory_list = q;
	}

	inline void * Alloc::Reallocate(void * p, size_t new_value, size_t old_value)
	{
		Deallocate(p, old_value);
		p = Allocate(new_value);
		return p;
	}

	inline size_t Alloc::M_align(size_t bytes)
	{
		if (bytes <= 512)
		{
			return bytes <= 256 ?
				bytes <= 128 ? Align128 : Align256
				: Align512;
		}
		return bytes <= 2048 ?
			bytes < 1024 ? Align1024 : Align2048
			: Align4096;
	}

	inline size_t Alloc::M_round_up(size_t bytes)// make bytes to be the interval,, etc: 7 to 8 , 13 to 16 ,215 to 256....
	{
		return(bytes + M_align(bytes) - 1 & ~(M_align(bytes) - 1));
	}

	inline size_t Alloc::M_memorylist_index(const size_t bytes)//only 0 to 128 has 16 nodes,others only have 8 nodes
	{
		if(bytes <= 512)
		{
			return bytes <= 256 ?
				bytes <= 128 ? ((bytes + Align128 - 1) / Align128 - 1) : (15 + (bytes + Align256 - 129) / Align256)
				: (23 + (bytes + Align512 - 257) / Align512);
		}
		return bytes <=2048  ?
			bytes < 1024 ? (31 + (bytes + Align1024 - 513) / Align1024) : (39 + (bytes + Align2048 - 1025) / Align2048)
			: (47 +( bytes + Align4096 - 2049) / Align4096);
	}

	inline void* Alloc::M_refill(size_t n)//reffill memory list
	{
		size_t blocks = 10;
		auto c = M_chunk_alloc(n, blocks);
		Memory_List* my_memory_list;
		Memory_List* result, *next,*cur;
		if (blocks == 1) return c;//only one block ,giving the block to the caller.
								  //others ,give one block to the caller ,and the rest of it are filled into memory_list
		my_memory_list = memory_list[M_memorylist_index(n)];
		result = reinterpret_cast<Memory_List*>(c);
		my_memory_list = next = reinterpret_cast<Memory_List*>(c + n);
		for(size_t i = 0; ;++i)
		{
			cur = next;
			next = reinterpret_cast<Memory_List*>(reinterpret_cast<char*>(next)+n);
			if (blocks - 1 == i)
			{
				cur->next = nullptr;
				break;
			}
			else
				cur->next = next;

		}
		return result;
	}

	inline char* Alloc::M_chunk_alloc(size_t size, size_t& nblocks)
	{
		char* result;
		size_t needed_bytes = size* nblocks;
		size_t pool_bytes = last_memory - first_memory;
		if(pool_bytes >= needed_bytes)
		{
			result = first_memory;
			last_memory += needed_bytes;
			return result;
		}
		else if (pool_bytes >= size)  // can afford enough blocks
		{
			nblocks = pool_bytes / size;
			result = first_memory;
			last_memory += size * nblocks;
			return result;
		}
		//threre is no enough memory for size  , expand the pool
		else
		{
			if(pool_bytes > 0)//add the rest of pool_bytes to memory_list
			{
				Memory_List* my_memory_list = memory_list[M_memorylist_index(pool_bytes)];
				reinterpret_cast<Memory_List*>(first_memory)->next = my_memory_list;
				my_memory_list = reinterpret_cast<Memory_List*>(first_memory);
			}
		}
		//ask for heap
		size_t bytes_get = (needed_bytes << 1) + M_round_up(heap_size >> 4);
		first_memory = (char*)std::malloc(bytes_get);
		if(!first_memory)//heap memory is out
		{
			Memory_List* my_memory_list, *p;
			//search for unused list node
			for(size_t i = size; i < SmallObejectBytes;i += M_align(i))
			{
				my_memory_list = memory_list[M_memorylist_index(i)];
				p = my_memory_list;
				if (p)// exist 
				{
					my_memory_list = p->next;
					first_memory = reinterpret_cast<char*>(p);
					last_memory = first_memory + i;
					return M_chunk_alloc(size,nblocks);
				}
			}
			printf("out of memory");
			last_memory = nullptr;
			throw std::bad_alloc();

		}
		last_memory = first_memory + bytes_get;
		heap_size += bytes_get;
		return M_chunk_alloc(size, nblocks);
		
	}
}
