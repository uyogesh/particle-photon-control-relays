#include<iostream>

class NewClass{

    public:
        std::string name;

        NewClass(std::string name){
            this->name = name;
        }

        std::string showName(){
            return this->name;
        }
};

NewClass* generateNewClasses(){
    NewClass classes[10];
    int i=0;
    for(i = 0; i<10; i++){
        NewClass clasy = NewClass("yog");
        classes[i] = clasy;
    }
    return classes;
}


int main(void){
    NewClass* newClass;
    newClass = generateNewClasses();
    for(int k=0; k<10; k++){
        std::cout<<newClass[k].showName()<<std::endl;
    }
    return 0;
}
