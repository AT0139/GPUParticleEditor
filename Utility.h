﻿#pragma once

namespace Utility
{
	template<typename T>
	std::shared_ptr<T> GetCastThis()
	{
		auto Ptr = std::dynamic_pointer_cast<T>(this);
		if (Ptr)
		{
			return Ptr;
		}
		else
		{
			assert(Ptr);
		}
		return nullptr;
	}
}