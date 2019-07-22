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

void my_sleep(unsigned msec) {
    struct timespec req, rem;
    int err;
    req.tv_sec = msec / 1000;
    req.tv_nsec = (msec % 1000) * 1000000;
    while ((req.tv_sec != 0) || (req.tv_nsec != 0)) {
        if (nanosleep(&req, &rem) == 0)
            break;
        err = errno;
        // Interrupted; continue
        if (err == EINTR) {
            req.tv_sec = rem.tv_sec;
            req.tv_nsec = rem.tv_nsec;
        }
        // Unhandleable error (EFAULT (bad pointer), EINVAL (bad timeval in tv_nsec), or ENOSYS (function not supported))
        break;
    }
}
int main(){

	rs2::pipeline pipe;

	rs2::config cfg;
	cfg.enable_stream(RS2_STREAM_POSE);

	pipe.start(cfg);
	
	
	long numFrames = 0;
	std::chrono::high_resolution_clock m_clock;
	uint64_t start = std::chrono::duration_cast<std::chrono::microseconds>(m_clock.now().time_since_epoch()).count();
	uint64_t lastFrame = std::chrono::duration_cast<std::chrono::microseconds>(m_clock.now().time_since_epoch()).count();
	try{
		while(numFrames < 10000){			
			ofstream myfile;
			
			auto frames = pipe.wait_for_frames();

			auto f = frames.first_or_default(RS2_STREAM_POSE);
			auto pose_data = f.as<rs2::pose_frame>().get_pose_data();
			//Print position
			//std::cout << "\r" << "Device Position: " << std::setprecision(3) << std::fixed << pose_data.translation.x << " " << pose_data.translation.y << " " << pose_data.translation.z << " (meters)";
			//std::cout << "\r" << "Device Confidence Level (0-3): " << std::setprecision(1) << pose_data.tracker_confidence;
			uint64_t now = std::chrono::duration_cast<std::chrono::microseconds>(m_clock.now().time_since_epoch()).count();
			long secSinceStart = (now-start);
			//float frameRate = 1000000.0f/((float)(now-lastFrame));
			//printf("Frame: %d | Time: %d | Quarternion Rotation: %f, %f, %f, %f | Device Position: %fm, %fm, %fm  |   Confidence Level (0-3): %d\n", numFrames, secSinceStart, pose_data.rotation.x, pose_data.rotation.y, pose_data.rotation.z, pose_data.rotation.w, pose_data.translation.x, pose_data.translation.y, pose_data.translation.z, pose_data.tracker_confidence);			

			myfile.open("coordinateData.csv");
			myfile << numFrames<< "," << secSinceStart << "," << pose_data.rotation.x << "," << pose_data.rotation.y << "," << pose_data.rotation.z << "," << pose_data.rotation.w << "," << pose_data.translation.x << "," << pose_data.translation.y << "," << pose_data.translation.z << "," << pose_data.tracker_confidence << endl;
			myfile.close();
			lastFrame=now;
			numFrames++;
		}
	}
	catch(std::exception& e){
		cerr << "Device Disconnected!" << endl;
		return -1;
	}
	return 0;
}
