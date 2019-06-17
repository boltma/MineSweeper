#ifndef BLOCK_H
#define BLOCK_H

enum BLOCK_STATUS
{
	MINE = 0x1,
	MARK = 0x2,
	OPEN = 0x4,
	QUESTION = 0x8
};

// 方格信息
class Block
{
private:
	int flag;

public:
	Block();
	void PlaceMine();
	void MarkMine();
	void QuestionMine();
	void OpenMine();
	bool HasMine();
};

#endif // BLOCK_H