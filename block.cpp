#include "block.h"

Block::Block() : flag(0) {}

void Block::PlaceMine()
{
	flag |= MINE;
}

void Block::MarkMine()
{
	flag |= MARK;
}

void Block::QuestionMine()
{
	flag |= QUESTION;
	flag &= ~MARK;
}

void Block::OpenMine()
{
	flag |= OPEN;
	flag &= ~MARK;
	flag &= ~QUESTION;
}

bool Block::HasMine()
{
	return flag & MINE;
}
