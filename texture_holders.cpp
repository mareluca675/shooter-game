//#include "texture_holders.h"
//#include <cassert>
//
//void TextureHolders::load(Textures::ID id, const std::string& file_path) {
//	std::unique_ptr<sf::Texture> texture(new sf::Texture);
//	if (!texture->loadFromFile(file_path)) {
//		throw std::runtime_error("TextureHolder::load - Failed to load " + file_path);
//	}
//
//	auto inserted = textureMap.insert(std::make_pair(id, std::move(texture)));
//	assert(inserted.second); // Check if texture was added successfully
//}
//
//sf::Texture& TextureHolders::get(Textures::ID id) {
//	auto found = textureMap.find(id);
//	assert(found != textureMap.end()); // Check if texture exists
//	return (*found->second);
//}
//
//const sf::Texture& TextureHolders::get(Textures::ID id) const {
//	auto found = textureMap.find(id);
//	assert(found != textureMap.end()); // Check if texture exists
//	return (*found->second);
//}