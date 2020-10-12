#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#include "Buffer.h"

namespace Vulkan{


	class UniformBuffer : public Buffer {

	public:

		explicit UniformBuffer(uint64_t size, void* data = nullptr);

		~UniformBuffer();

		void OnCreate();
		void OnDestroy();

	};


}


#endif // !UNIFORMBUFFER_H
