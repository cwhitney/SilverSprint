#pragma once

#include <vector>

template<typename T>
class CheapCircBuffer {
public:
	CheapCircBuffer(const int &size = 60){
		mSize = size;
		mData.resize(mSize);
	}
            
	void reset(const T &defaultVal){
		for( auto &d : mData){
			d = defaultVal;
		}
	}
            
	void push(const T &val ){
		mData[dex] = val;
		if(++dex >= mSize) dex = 0;
	}
            
            
	const T& getAverage(){
		T t = 0;
		for( auto &d : mData){
			t += d;
		}
		mCurAverage = t / static_cast<double>(mSize);
		return mCurAverage;
	}
private:
	int dex = 0;
	int mSize = 30;
	T   mCurAverage;
            
	std::vector<T> mData;
};