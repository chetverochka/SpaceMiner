#ifndef __CHUNK_MAP_H__
#define __CHUNK_MAP_H__

#include <vector>

// Задел на будущее

/*
ChunkObjectDelegate это интерфейс предоставляющий методы для работы с сеткой чанков
*/
class ChunkObjectDelegate {
	//TODO: переименовать, сделать так чтобы GameObject наследовался от него, вынести в отдельный hpp файл

	virtual void setCellX(int x) = 0;
	virtual void setCellY(int y) = 0;
	
	virtual int getCellX() const = 0;
	virtual int getCellY() const = 0;
	
	virtual void setCell(int x, int y) = 0;
	virtual void getCell(int* x, int* y) const = 0;
};


class ChunkMap {
public:
	
private:
	// отказаться от CHUNK и сделать ChunkMap единственной точкой обработки и получения данных
	/*class Chunk {
	public:
		int posX, posY;
		std::vector<ChunkObjectDelegate*> objects;
	};*/
	// cellStepX, cellStepY: размер ячеек внутри чанков (не размер единичной ячейки)
	// std::vector<IChunkMapObject>
	// std::vector<Chunk> ...
};


#endif //!__CHUNK_MAP_H__
