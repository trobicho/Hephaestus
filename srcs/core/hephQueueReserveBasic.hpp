#pragma once

#include "hephQueueReserveInterface.hpp"

class	HephQueueReserveBasic: public HephQueueReserveInterface {
	public:
		HephResult	reserve(VkPhysicalDevice physicalDevice);

	protected:
		struct	QueueFamilyRating {
			float			rating;
			uint32_t	family;
			inline bool operator>(const QueueFamilyRating& rhs) const {return (this->rating < rhs.rating);}
			inline bool operator<(const QueueFamilyRating& rhs) const {return (this->rating > rhs.rating);}
		};
		void	rateQueueFamily();
		std::vector<std::vector<QueueFamilyRating>>	m_queueFamilyRatings;
};
