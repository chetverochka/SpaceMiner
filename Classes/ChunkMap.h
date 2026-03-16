#ifndef __CHUNK_MAP_H__
#define __CHUNK_MAP_H__

#include <vector>
#include "CCVec2i.h"

// Задел на будущее
/*
ChunkMap расчитана на универсальное использование в других проектах. 
Поэтому ниже готовая система позволяющая интегрировать расчёт чанков во все игры где это нужно.
Просто наследуйте Объекты и от ChunkObjectDelegate и добавляйте их в ChunkMap для работы с ними
*/


/*
ChunkObjectDelegate это интерфейс предоставляющий методы для работы с сеткой чанков
*/
class ChunkObjectDelegate {
	//TODO: переименовать, сделать так чтобы GameObject наследовался от него, вынести в отдельный hpp файл (completed)
public:
	virtual void setCellX(int x) = 0;
	virtual void setCellY(int y) = 0;
	
	virtual int getCellX() const = 0;
	virtual int getCellY() const = 0;
	
	virtual void setCell(int x, int y) = 0;
	virtual void getCell(int* x, int* y) const = 0;
};


class ChunkMap {
public:
	ChunkMap();
	ChunkMap(int cellsCountX, int cellsCountY);
	ChunkMap(const cocos2d::CCVec2i& cellsCount);

	cocos2d::CCVec2i getCellsCount() const;

	/*
	Выдаёт индекс (координаты) чанка по глобальным координатам ячейки. Например:
	Чанк содержит сетку ячеек 12х12
	Глобальная координата ячейки: (13, 6)

	-> Подходящий чанк для этой ячейки будет под индексом: (1, 0)
	*/
	cocos2d::CCVec2i getChunkCoords(const cocos2d::CCVec2i& globalCellCoords) const;

	/*
	Конвертирует глобальные координаты ячейки в 
	локальные координаты относителя координат чанка с заданным индексом (координатами)
	*/
	cocos2d::CCVec2i convertGlobalToLocal(const cocos2d::CCVec2i& globalCellCoords, const cocos2d::CCVec2i& chunkCoords) const;

	/*
	Конвертирует локальные координаты ячейки в 
	глобальные относительно чанка с заданным индексом (координатами)
	*/
	cocos2d::CCVec2i convertLocalToGlobal(const cocos2d::CCVec2i& localCellCoords, const cocos2d::CCVec2i& chunkCoords) const;
	
	bool contains(ChunkObjectDelegate* object) const;
	void add(ChunkObjectDelegate* object);
	void remove(ChunkObjectDelegate* object);
	void clear();

	void updateObject(std::vector<ChunkObjectDelegate*>& range); // base method for updaing object calculations in chunk
	void updateObject(ChunkObjectDelegate* object); 
	void updateAllObjects(); // all objects

	cocos2d::CCVec2i getObjectChunk(ChunkObjectDelegate* object) const;

	std::vector<ChunkObjectDelegate*> getObjects();
	std::vector<ChunkObjectDelegate*> getObjects(const cocos2d::CCVec2i& chunkCoords);
private:
	struct Chunk { // internal structure (container)
		cocos2d::CCVec2i position;
		std::vector<ChunkObjectDelegate*> objects;
	};

	Chunk* getChunkAt(const cocos2d::CCVec2i& chunkCoords);

	std::vector<ChunkObjectDelegate*> m_objects;
	cocos2d::CCVec2i m_cellsCount; // cells count per ONE chunk
	std::vector<Chunk> m_chunks;
};


#endif //!__CHUNK_MAP_H__
