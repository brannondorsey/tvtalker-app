#pragma once

#include "ofMain.h"
#include "ofxJSONRPC.h"
#include "ClosedCaptionManager.h"

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
        void jsonRPCTest(ofx::JSONRPC::MethodArgs& args);
    
        bool bRequestPlayVideoMessageRecieved;
        bool bMouseShowing;
    
        std::string videoPath;
    
        ofVideoPlayer messagePlayer;
        ofVideoPlayer backgroundPlayer;
    
        mutable ofMutex mutex;
    
        ofx::HTTP::BasicJSONRPCServer::SharedPtr server;
    
        ClosedCaptionManager ccManager;
};
