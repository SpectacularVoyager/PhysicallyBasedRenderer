#pragma once
#include<vector>

struct layoutElement{
	int id;
	int size;
	layoutElement(int id,int size){
		this->id=id;
		this->size=size;
	}
};

class VertexLayout{
	private:
		int stride=0;
		std::vector<layoutElement> layout;
	public:
		void addLayout(int id,int size);
		void setLayout();
		inline int getStride(){return stride;}
};

