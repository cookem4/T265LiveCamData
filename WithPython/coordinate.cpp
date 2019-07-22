#include <librealsense2/rs.hpp>
#include <librealsense2/rsutil.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <fstream>
#include <chrono>
#include <ctime>
#include <cstdint>
#include <stdlib.h>
#include <errno.h>

using namespace rs2;
using namespace std;

rs2::pipeline pipe;
rs2::config cfg;
int numFrames = 0;
extern "C"
void configureCamera(){
	cfg.enable_stream(RS2_STREAM_POSE);
	pipe.start(cfg);
	
}
extern "C"
double * getCamData(){		
	
	long numFrames = 0;
	std::chrono::high_resolution_clock m_clock;
	uint64_t start = std::chrono::duration_cast<std::chrono::microseconds>(m_clock.now().time_since_epoch()).count();
	uint64_t lastFrame = std::chrono::duration_cast<std::chrono::microseconds>(m_clock.now().time_since_epoch()).count();			
	auto frames = pipe.wait_for_frames();

	auto f = frames.first_or_default(RS2_STREAM_POSE);
	auto pose_data = f.as<rs2::pose_frame>().get_pose_data();
	uint64_t now = std::chrono::duration_cast<std::chrono::microseconds>(m_clock.now().time_since_epoch()).count();
	long secSinceStart = (now-start);
	double returnVal[10];
	returnVal[0] =1.0* numFrames;
	returnVal[1] =1.0* secSinceStart;
	returnVal[2] = 1.0*pose_data.rotation.x;
	returnVal[3] = 1.0*pose_data.rotation.y;
	returnVal[4] = 1.0*pose_data.rotation.z;
	returnVal[5] = 1.0*pose_data.rotation.w;
	returnVal[6] = 1.0*pose_data.translation.x;
	returnVal[7] = 1.0*pose_data.translation.y;
	returnVal[8] = 1.0*pose_data.translation.z;
	returnVal[9] = 1.0*pose_data.tracker_confidence;

	lastFrame=now;
	numFrames++;
	return 0;
}
