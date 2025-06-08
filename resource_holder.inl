#include <cassert>

template <typename Resource, typename Identifier>
void ResourceHolder< Resource, Identifier>::load(Identifier id, const
	const std::string& file_path) {
	std::unique_ptr<Resource> resource(new Resource());
	if (!resource->loadFromFile(file_path)) {
		throw std::runtime_error("ResourceHolder::load - Failed to load " + file_path);
	}

	auto inserted = resourceMap.insert(std::make_pair(id, std::move(resource)));
	assert(inserted.second); // Check if resource was added successfully
}

template <typename Resource, typename Identifier>
Resource& ResourceHolder< Resource, Identifier>::get(Identifier id) {
	auto found = resourceMap.find(id);
	assert(found != resourceMap.end()); // Check if resource exists
	return (*found->second);
}

template <typename Resource, typename Identifier>
const Resource& ResourceHolder< Resource, Identifier>::get(Identifier id) const {
	auto found = resourceMap.find(id);
	assert(found != resourceMap.end()); // Check if resource exists
	return (*found->second);
}