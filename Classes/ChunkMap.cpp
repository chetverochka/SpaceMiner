#include "ChunkMap.h"

USING_NS_CC;

ChunkMap::ChunkMap() : ChunkMap(12, 12) {

}

ChunkMap::ChunkMap(int cellsCountX, int cellsCountY) {
	m_cellsCount.x = cellsCountX;
	m_cellsCount.y = cellsCountY;
	m_chunks = {};
	m_objects = {};
}

ChunkMap::ChunkMap(const cocos2d::CCVec2i& cellsCount) : ChunkMap(cellsCount.x, cellsCount.y) {}

CCVec2i ChunkMap::getCellsCount() const {
	return m_cellsCount;
}

CCVec2i ChunkMap::getChunkCoords(const CCVec2i& globalCellCoords) const {
	CCVec2i ret;
	
	ret.x = (int)(std::floorf((float)globalCellCoords.x / (float)m_cellsCount.x));
	ret.y = (int)(std::floorf((float)globalCellCoords.y / (float)m_cellsCount.y));

	return ret;
}

cocos2d::CCVec2i ChunkMap::convertGlobalToLocal(
	const cocos2d::CCVec2i& globalCellCoords, 
	const cocos2d::CCVec2i& chunkCoords
) const {
	CCVec2i ret;

	ret.x = globalCellCoords.x - (chunkCoords.x * m_cellsCount.x);
	ret.y = globalCellCoords.y - (chunkCoords.y * m_cellsCount.y);

	return ret;
}

cocos2d::CCVec2i ChunkMap::convertLocalToGlobal(
	const cocos2d::CCVec2i& localCellCoords,
	const cocos2d::CCVec2i& chunkCoords
) const {
	CCVec2i ret;

	ret.x = localCellCoords.x + (chunkCoords.x * m_cellsCount.x);
	ret.y = localCellCoords.y + (chunkCoords.y * m_cellsCount.y);

	return ret;
}


bool ChunkMap::contains(ChunkObjectDelegate* object) const {
	return std::find(m_objects.begin(), m_objects.end(), object) != m_objects.end();
}
void ChunkMap::add(ChunkObjectDelegate* object) {
	if (contains(object))
		return;

	m_objects.push_back(object);
	updateObject(object);
}
void ChunkMap::remove(ChunkObjectDelegate* object) {
	if (!contains(object))
		return;

	m_objects.erase(std::find(m_objects.begin(), m_objects.end(), object));
}

void ChunkMap::clear() {
	CCAssert(false, "IMPLEMENT ME");
}

void ChunkMap::updateObject(std::vector<ChunkObjectDelegate*>& range) {
	CCAssert(false, "IMPLEMENT ME");

	for (int i = 0; i < range.size(); i++) {
		ChunkObjectDelegate* object = range[i];

		const CCVec2i objectCell(object->getCellX(), object->getCellY());
		const CCVec2i newChunk = getChunkCoords(objectCell);
		

	}
}

void ChunkMap::updateObject(ChunkObjectDelegate* object) {
	updateObject({ object });
}

void ChunkMap::updateAllObjects() {
	updateObject(m_objects);
}

cocos2d::CCVec2i ChunkMap::getObjectChunk(ChunkObjectDelegate* object) const {
	CCAssert(false, "IMPLEMENT ME");
	return CCVec2i();
}

std::vector<ChunkObjectDelegate*> ChunkMap::getObjects() {
	return m_objects;
}

std::vector<ChunkObjectDelegate*> ChunkMap::getObjects(const CCVec2i& chunkCoords) {
	ChunkMap::Chunk* targetChunk = getChunkAt(chunkCoords);
	if (targetChunk) {
		return targetChunk->objects;
	}
	else {
		return {};
	}
}

ChunkMap::Chunk* ChunkMap::getChunkAt(const CCVec2i& chunkCoords) {
	for (int i = 0; i < m_chunks.size(); i++) {
		Chunk& chunk = m_chunks[i];
		if (
			chunk.position.x == chunkCoords.x && 
			chunk.position.y == chunkCoords.y
		) {
			return &chunk;
		}
	}
	return NULL;
}