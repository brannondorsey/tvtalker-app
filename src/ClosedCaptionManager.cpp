//
//  ClosedCaptionManager.cpp
//  TVTalker
//
//  Created by bdorse on 12/4/14.
//
//

#include "ClosedCaptionManager.h"

ClosedCaptionManager::ClosedCaptionManager():
_closedCaptions(Json::Value::null),
_counter(0),
_curText(""),
_bIsShowing(false),
_bEnabled(true),
_yPos(0),
_xPos(30),
_padding(10),
_maxCharsPerLine(0) {
    
    ofAddListener(_timer.TIMER_STARTED , this, &ClosedCaptionManager::_onTimerStarted);
    ofAddListener(_timer.TIMER_COMPLETE , this, &ClosedCaptionManager::_onTimerComplete);
    
    _timecode.setFPS(59.97);
    
    _font.loadFont("5902.ttf", 48);
    
    _yPos = ofGetHeight() - 50;
    
    setMaxCharsPerLine();
}

ClosedCaptionManager::~ClosedCaptionManager() {
    ofAddListener(_timer.TIMER_STARTED , this, &ClosedCaptionManager::_onTimerStarted);
    ofRemoveListener(_timer.TIMER_COMPLETE, this, &ClosedCaptionManager::_onTimerComplete);
}

void ClosedCaptionManager::setCaptions(Json::Value captions) {
    _closedCaptions = captions;
}

void ClosedCaptionManager::start() {
    
    if (_closedCaptions != Json::Value::null) {
        
        _counter = 0;
        _bIsShowing = true;
        _step();
    }
}

void ClosedCaptionManager::update() {
    _timer.update();
}

void ClosedCaptionManager::draw() {
    
//    if (isShowing() &&
//        isEnabled()) {
    
        std::string text = _curText;
        int numRows = 0;
    
        if (text.size() > _maxCharsPerLine) {
            text = _wordWrap(text, _maxCharsPerLine);
            numRows = std::count(text.begin(), text.end(), '\n');
        }
    
        int yPos = _yPos - (numRows * _font.getLineHeight());
        
        ofPushStyle();

        ofRectangle boundingBox = _font.getStringBoundingBox(text, _xPos, yPos);
        _applyPadding(boundingBox);
    
        ofSetColor(0);
        ofRect(boundingBox);
    
        ofSetColor(255);
        _font.drawString(text, _xPos, yPos);
        
        ofPopStyle();
//    }
}

void ClosedCaptionManager::setMaxCharsPerLine() {
    
    std::string str;
    str.resize(1000, 'T');
    
    for (int i = 0; i < str.size(); i++) {
        
        std::string sub = str.substr(0, i + 1);
        ofRectangle bounds = _font.getStringBoundingBox(sub, _xPos, _yPos);
        if (bounds.width > ofGetWidth()) {
            _maxCharsPerLine = max(i - 1, 0);
            break;
        }
    }
}

void ClosedCaptionManager::setEnabled(bool enable) {
    _bEnabled = enable;
}

bool ClosedCaptionManager::isShowing() {
    return _bIsShowing;
}

bool ClosedCaptionManager::isEnabled() {
    return _bEnabled;
}

void ClosedCaptionManager::_step() {

    if (_counter < _closedCaptions.size()) {
        
        _curText = _closedCaptions[_counter]["text"].asString();
        
        std::string in = _closedCaptions[_counter]["in"].asString();
        std::string out = _closedCaptions[_counter]["out"].asString();
        
        ofStringReplace(in, ".", ":");
        ofStringReplace(out, ".", ":");
        
        unsigned long inMillis = _timecode.millisForTimecode(in);
        unsigned long outMillis = _timecode.millisForTimecode(out);
        unsigned long lengthMillis = outMillis - inMillis;
        
        _timer.setup(lengthMillis);
        _timer.start(false);
        
        _counter++;
    
    } else {
        _bIsShowing = false;
    }
}

void ClosedCaptionManager::_applyPadding(ofRectangle& bounds) {
    bounds.x -= _padding;
    bounds.width += _padding * 2;
    bounds.y -= _padding;
    bounds.height += _padding * 2;
}

void ClosedCaptionManager::_onTimerStarted(int &args) {
    
}

void ClosedCaptionManager::_onTimerComplete(int &args) {
    _step();
}

// taken from http://www.cplusplus.com/forum/beginner/132223/
std::string ClosedCaptionManager::_wordWrap(std::string text, unsigned int per_line) {
    
    unsigned line_begin = 0;
    
    while (line_begin < text.size())
    {
        const unsigned ideal_end = line_begin + per_line ;
        unsigned line_end = ideal_end <= text.size() ? ideal_end : text.size()-1;
        
        if (line_end == text.size() - 1)
            ++line_end;
        else if (std::isspace(text[line_end]))
        {
            text[line_end] = '\n';
            ++line_end;
        }
        else    // backtrack
        {
            unsigned end = line_end;
            while ( end > line_begin && !std::isspace(text[end]))
                --end;
            
            if (end != line_begin)
            {
                line_end = end;
                text[line_end++] = '\n';
            }
            else
                text.insert(line_end++, 1, '\n');
        }
        
        line_begin = line_end;
    }
    
    return text;
}