#pragma once

#include "ofMain.h"
#include "ofxJSONRPC.h"
#include "ClosedCaptionManager.h"

//#define DEBUG

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void loadAndPlayMessageVideo(std::string filePath);
        void requestPlayVideo(ofx::JSONRPC::MethodArgs& args);
        void backgroundVideoOn(ofx::JSONRPC::MethodArgs& args);
        void backgroundVideoOff(ofx::JSONRPC::MethodArgs& args);
    
        bool bRequestPlayVideoMessageRecieved;
        bool bMouseShowing;
        bool bMessageVideoPlaying;
        bool bDrawBackgroundVideo;
    
        std::string videoPath;
    
        ofVideoPlayer messagePlayer;
        ofVideoPlayer backgroundPlayer;
    
        mutable ofMutex mutex;
    
        ofx::HTTP::BasicJSONRPCServer::SharedPtr server;
    
        ClosedCaptionManager ccManager;
};
