#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(60);
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
    
    server->registerMethod("backgroundVideoOn",
                           "Display the background video",
                           this,
                           &ofApp::backgroundVideoOn);
    
    server->registerMethod("backgroundVideoOff",
                           "Don't display the background video",
                           this,
                           &ofApp::backgroundVideoOff);
    
    server->start();

    std::string filePath = "/Volumes/Lexar/nightly_news.mp4";
    backgroundPlayer.loadMovie(filePath);
    backgroundPlayer.setLoopState(OF_LOOP_NORMAL);
    backgroundPlayer.setVolume(0);
    backgroundPlayer.play();
    
    bRequestPlayVideoMessageRecieved = false;
    bDrawBackgroundVideo = false;
    bMouseShowing = true;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ccManager.update();
    
    backgroundPlayer.update();
    
    if (bRequestPlayVideoMessageRecieved) {
        
        loadAndPlayMessageVideo(videoPath);
        bRequestPlayVideoMessageRecieved = false;
    }
    
    if (messagePlayer.isLoaded()) {
        
        messagePlayer.update();
        
        if (messagePlayer.getIsMovieDone()) {
            messagePlayer.close();
            bMessageVideoPlaying = false;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (bDrawBackgroundVideo) {
        backgroundPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    if (messagePlayer.isLoaded()) {
        messagePlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    ccManager.draw();
    
#ifdef DEBUG
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate(), 0) + " FPS", 20, 20);
#endif
}

void ofApp::loadAndPlayMessageVideo(std::string filePath) {
    
    ofScopedLock lock(mutex);
    if (messagePlayer.isLoaded()) messagePlayer.close();
    messagePlayer.loadMovie(filePath);
    messagePlayer.setLoopState(OF_LOOP_NONE);
    messagePlayer.play();
    ccManager.start();
}

void ofApp::requestPlayVideo(ofx::JSONRPC::MethodArgs& args) {
    
    if (args.params.isMember("message") &&
        args.params.isMember("videoPath") &&
        args.params.isMember("closedCaptions")) {
    
        std::string message = args.params["message"].asString();
        std::string path = args.params["videoPath"].asString();
        ccManager.setCaptions(args.params["closedCaptions"]);
            
        ofFile videoFile(path);
            
        if (videoFile.exists()) {
                
            videoPath = videoFile.path();
                
            if (!bMessageVideoPlaying) {
                bMessageVideoPlaying = true;
                bRequestPlayVideoMessageRecieved = true;
            } else {
                args.error["code"] = 2; // video already playing
                args.error["message"] = "Message video already playing";
                ofLogWarning("ofApp::requestPlayVideo") << "Video play request denied. A message video already playing.";
            }
        }
        else {
            ofLogError("ofApp::requestPlayVideo") << "The requested video does not exist";
        }
        
    } else {
        ofLogError("ofApp::requestPlayVideo") << "Missing required parameters";
        args.error["code"] = 1;
        args.error["message"] = "Missing parameters";
    }
}

void ofApp::backgroundVideoOn(ofx::JSONRPC::MethodArgs& args) {
    bDrawBackgroundVideo = true;
}

void ofApp::backgroundVideoOff(ofx::JSONRPC::MethodArgs& args) {
    bDrawBackgroundVideo = false;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if (key == ' ') {
        
        // test play video
        loadAndPlayMessageVideo("/Users/bdorse/Documents/code/tvtalker/node/data/DocumentRoot/media/video.mov");
   
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
