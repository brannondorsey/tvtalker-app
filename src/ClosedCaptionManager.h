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
    void setFadeTimeout(unsigned long millis);
    void setMaxCharsPerLine();
    
    bool isEnabled();
    bool isShowing();
    
protected:
    
    void _applyPadding(ofRectangle& bounds);
    void _step();
    void _onWordTimerStarted(int& args);
    void _onWordTimerComplete(int& args);
    void _onFadeTimerStarted(int& args);
    void _onFadeTimerComplete(int& args);
    
    std::string _wordWrap(std::string text, unsigned int charsPerLine);
    
    int _counter;
    int _yPos;
    int _xPos;
    int _padding;
    int _maxCharsPerLine;
    unsigned long _fadeTimeout;
    bool _bIsShowing;
    bool _bEnabled;
    std::string _curText;
    Json::Value _closedCaptions;
    ofTrueTypeFont _font;
    ofxSimpleTimer _wordTimer;
    ofxSimpleTimer _fadeTimer;
    ofxTimecode _timecode;
    
};

#endif /* defined(__TVTalker__ClosedCaptionManager__) */
