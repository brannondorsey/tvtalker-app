#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofBackground(0);
    
    ofx::HTTP::BasicJSONRPCServerSettings settings;
    settings.setPort(8954);

    ofLogVerbose();
    
    // Initialize the server.
    server = ofx::HTTP::BasicJSONRPCServer::makeShared(settings);
    
    server->registerMethod("requestPlayVideo",
                           "Play the requested video",
                           this,
                           &ofApp::requestPlayVideo);
    
    server->registerMethod("jsonRPCTest",
                           "test",
                           this,
                           &ofApp::jsonRPCTest);
    
    server->start();

    std::string filePath = "/Volumes/Untitled/hdhomerun/video/programs/compressed/2014-11-09_WBBMDT_CBS-2-News-at-10PM.mp4";
    backgroundPlayer.loadMovie(filePath);
    backgroundPlayer.setLoopState(OF_LOOP_NORMAL);
    backgroundPlayer.setVolume(0);
    backgroundPlayer.play();
    
    bRequestPlayVideoMessageRecieved = false;
    bMouseShowing = true;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ccManager.update();
    
    backgroundPlayer.update();
    
    if (bRequestPlayVideoMessageRecieved) {
        loadAndPlayMessageVideo("/Users/bdorse/Documents/code/hdhomerun/tools/node/data/DocumentRoot/media/video.mov");
        bRequestPlayVideoMessageRecieved = false;
    }
    
    if (messagePlayer.isLoaded()) {
        
        messagePlayer.update();
        
        if (messagePlayer.getIsMovieDone()) {
            messagePlayer.close();
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    backgroundPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    if (messagePlayer.isLoaded()) {
        messagePlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    ccManager.draw();
}

void ofApp::loadAndPlayMessageVideo(std::string filePath) {
    
    ofScopedLock lock(mutex);
    if (messagePlayer.isLoaded()) messagePlayer.close();
    cout << filePath << endl;
    messagePlayer.loadMovie(filePath);
    messagePlayer.setLoopState(OF_LOOP_NONE);
    messagePlayer.play();
    ccManager.start();
}

void ofApp::requestPlayVideo(ofx::JSONRPC::MethodArgs& args) {
    
    if (!args.error) {
    
        std::string message = args.params["message"].asString();
        std::string path = args.params["videoPath"].asString();
        ccManager.setCaptions(args.params["closedCaptions"]);
        
        ofFile videoFile(path);
        
        if (videoFile.exists()) {
            videoPath = videoFile.path();
            bRequestPlayVideoMessageRecieved = true;
        }
        else {
            ofLogError("ofApp::requestPlayVideo") << "The requested video does not exist";
        }
        
    } else {
        ofLogError("ofApp::requestPlayVideo") << "JSONRPC error";
        cout << args.error.toStyledString() << endl;
    }
}

void ofApp::jsonRPCTest(ofx::JSONRPC::MethodArgs& args) {
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if (key == ' ') {
        
        loadAndPlayMessageVideo("/Users/bdorse/Documents/code/hdhomerun/tools/node/data/DocumentRoot/media/video.mov");
   
    } else if (key == 'f') {
        
        ofToggleFullscreen();
    
    } else if (key == 'm') {
        
        bMouseShowing = !bMouseShowing;
        
        if (bMouseShowing) ofShowCursor();
        else ofHideCursor();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    ccManager.setMaxCharsPerLine();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
