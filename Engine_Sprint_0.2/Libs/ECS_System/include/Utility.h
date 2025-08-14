#ifndef Utility_H
#define Utility_H



namespace zecs
{
	class IndexGetter
	{
	public:
		template<typename T>
		static uint32_t Get()
		{
			static uint32_t id = curIdx++;
			return id;
		}

	private:
		static uint32_t curIdx;
	};
}


#endif // !Utility
