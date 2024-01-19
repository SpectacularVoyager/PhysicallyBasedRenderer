#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include"include/VertexLayout.h"

void VertexLayout::addLayout(int id,int size){
	layout.push_back(layoutElement(id,size));
	this->stride+=size;
}

void VertexLayout::setLayout(){
	int sum=0;
	for(auto& element:layout){
		glEnableVertexAttribArray(element.id);
		glVertexAttribPointer(element.id, element.size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(sizeof(float)*sum));
		sum+=element.size;
	}
}
