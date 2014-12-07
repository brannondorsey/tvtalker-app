//
//  ClosedCaptionManager.h
//  TVTalker
//
//  Created by bdorse on 12/4/14.
//
//

#ifndef __TVTalker__ClosedCaptionManager__
#define __TVTalker__ClosedCaptionManager__

#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxSimpleTimer.h"
#include "ofxTimecode.h"

class ClosedCaptionManager {
public:
    
    ClosedCaptionManager();
    ~ClosedCaptionManager();
    
    void setCaptions(Json::Value captions);
    void start();
    void update();
    void draw();
    void setEnabled(bool enable);
    void setMaxCharsPerLine();
    
    bool isEnabled();
    bool isShowing();
    
protected:
    
    void _applyPadding(ofRectangle& bounds);
    void _step();
    void _onTimerStarted(int& args);
    void _onTimerComplete(int& args);
    
    std::string _wordWrap(std::string text, unsigned int charsPerLine);
    
    int _counter;
    int _yPos;
    int _xPos;
    int _padding;
    int _maxCharsPerLine;
    bool _bIsShowing;
    bool _bEnabled;
    std::string _curText;
    Json::Value _closedCaptions;
    ofTrueTypeFont _font;
    ofxSimpleTimer _timer;
    ofxTimecode _timecode;
    
};

#endif /* defined(__TVTalker__ClosedCaptionManager__) */
