#pragma once

#include <vector>
#include <utility> // for std::pair

class  CircBuffer {
public:
	CircBuffer(const int &size){
		mDataVec.resize(size);
		for( auto d : mDataVec){
			d = {0, 0};
		}
	}
            
	void reset()
	{
		for( int i=0; i<mDataVec.size(); i++){
			mDataVec[i] = {0.0, 0.0};
		}
                
		bFirstRun = true;
	}
            
	//! Args are a pair, where first = the current time in milliseconds, second = the number of ticks since that last update
	void add( const std::pair<int, int> &timeAndTicks ){
		if(bFirstRun){
			for( auto d : mDataVec){
				d = {timeAndTicks.first, 0};
			}
			bFirstRun = false;
		}
                
		mDataVec[dex] = timeAndTicks;
		if(++dex >= mDataVec.size())
			dex = 0;
	}
            
	//! Returns the average number of ticks per second in the recorded time period.
	double getAverageTicksSec()
	{
		int ld = (dex - 1 < 0) ? int(mDataVec.size()) - 1 : dex - 1;
		int early = (ld + 1) >= mDataVec.size() ? 0 : ld + 1;
		int earliestTimeMs = mDataVec[early].first;
		int latestTimeMs = mDataVec[ld].first;
		double dtSecs = (latestTimeMs - earliestTimeMs) / 1000.0;
                
		float totalTicks = 0.0;
		for( auto d : mDataVec){
			if(d.second > 0.0){
				totalTicks += d.second;
			}
		}
		return totalTicks / dtSecs;
	}
            
protected:
	int dex = 0;
	bool bFirstRun = true;
	std::vector< std::pair<int, int> > mDataVec;
};