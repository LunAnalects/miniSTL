#ifndef MINISTL_MEMORY_HPP
#define MINISTL_MEMORY_HPP

#include "copy.hpp"
#include "types.hpp"
#include "internal/pointer.hpp"
#include "unique_ptr.hpp"

namespace ministl
{
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	class reference_count
	{
	public:
		reference_count():sharedCount(1), weakCount(1){}


	private:
		uint32_t sharedCount;
		uint32_t weakCount;

	};








	template<typename T>
	class shared_ptr
	{
		
	};







}//ministl


#endif //MINISTL_MEMORY_HPP