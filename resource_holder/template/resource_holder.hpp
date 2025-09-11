#include <SFML/Graphics.hpp>

template <typename Resource, typename Identifier>
class ResourceHolder {
public:
	void load(Identifier id, const std::string& file_path);
	Resource& get(Identifier id);
	const Resource& get(Identifier id) const;
private:
	std::map<Identifier, std::unique_ptr<Resource>> resourceMap;
};

#include "resource_holder.inl"