/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "cocostudio/CCActionObject.h"
#include "cocostudio/DictionaryHelper.h"

using namespace cocos2d;

namespace cocostudio {

ActionObject::ActionObject()
: _actionNodeList(NULL)
, _name("")
, _loop(false)
, _bPause(false)
, _bPlaying(false)
, _fUnitTime(0.1f)
, _currentTime(0.0f)
, _pScheduler(NULL)
{
	_actionNodeList = Array::create();
	_actionNodeList->retain();
	_pScheduler = Director::getInstance()->getScheduler();
	CC_SAFE_RETAIN(_pScheduler);
}

ActionObject::~ActionObject()
{
	_actionNodeList->removeAllObjects();
	_actionNodeList->release();
	CC_SAFE_RELEASE(_pScheduler);
}

void ActionObject::setName(const char* name)
{
	_name.assign(name);
}
const char* ActionObject::getName()
{
	return _name.c_str();
}

void ActionObject::setLoop(bool bLoop)
{
	_loop = bLoop;
}
bool ActionObject::getLoop()
{
	return _loop;
}

void ActionObject::setUnitTime(float fTime)
{
	_fUnitTime = fTime;
	auto nodeNum = _actionNodeList->count();
	for ( int i = 0; i < nodeNum; i++ )
	{
		ActionNode* actionNode = (ActionNode*)_actionNodeList->getObjectAtIndex(i);
		actionNode->setUnitTime(_fUnitTime);
	}
}
float ActionObject::getUnitTime()
{
	return _fUnitTime;
}

float ActionObject::getCurrentTime()
{
	return _currentTime;
}

void ActionObject::setCurrentTime(float fTime)
{
	_currentTime = fTime;
}

bool ActionObject::isPlaying()
{
	return _bPlaying;
}

void ActionObject::initWithDictionary(JsonDictionary *dic,Object* root)
{
    setName(DICTOOL->getStringValue_json(dic, "name"));
    setLoop(DICTOOL->getBooleanValue_json(dic, "loop"));
	setUnitTime(DICTOOL->getFloatValue_json(dic, "unittime"));
    int actionNodeCount = DICTOOL->getArrayCount_json(dic, "actionnodelist");
    for (int i=0; i<actionNodeCount; i++) {
        ActionNode* actionNode = new ActionNode();
		actionNode->autorelease();
        JsonDictionary* actionNodeDic = DICTOOL->getDictionaryFromArray_json(dic, "actionnodelist", i);
        actionNode->initWithDictionary(actionNodeDic,root);
		actionNode->setUnitTime(getUnitTime());
        _actionNodeList->addObject(actionNode);
		CC_SAFE_DELETE(actionNodeDic);
    }
}

void ActionObject::addActionNode(ActionNode* node)
{
	if (node == NULL)
	{
		return;
	}
	_actionNodeList->addObject(node);
	node->setUnitTime(_fUnitTime);
}
void ActionObject::removeActionNode(ActionNode* node)
{
	if (node == NULL)
	{
		return;
	}
	_actionNodeList->removeObject(node);
}

void ActionObject::play()
{
    stop();
	this->updateToFrameByTime(0.0f);
	auto frameNum = _actionNodeList->count();
	for ( int i = 0; i < frameNum; i++ )
	{
		ActionNode* actionNode = (ActionNode*)_actionNodeList->getObjectAtIndex(i);
		actionNode->playAction();
	}
	if (_loop)
	{
		_pScheduler->scheduleSelector(schedule_selector(ActionObject::simulationActionUpdate), this, 0.0f , kRepeatForever, 0.0f, false);
	}
}

void ActionObject::pause()
{
	_bPause = true;
}

void ActionObject::stop()
{
	auto frameNum = _actionNodeList->count();

	for ( int i = 0; i < frameNum; i++ )
	{
		ActionNode* actionNode = (ActionNode*)_actionNodeList->getObjectAtIndex(i);
		actionNode->stopAction();
	}

	_pScheduler->unscheduleSelector(schedule_selector(ActionObject::simulationActionUpdate), this);
	_bPause = false;
}

void ActionObject::updateToFrameByTime(float fTime)
{
	_currentTime = fTime;

	auto nodeNum = _actionNodeList->count();

	for ( int i = 0; i < nodeNum; i++ )
	{
		ActionNode* actionNode = (ActionNode*)_actionNodeList->getObjectAtIndex(i);

		actionNode->updateActionToTimeLine(fTime);
	}
}

void ActionObject::simulationActionUpdate(float dt)
{
	if (_loop)
	{
		bool isEnd = true;
		auto nodeNum = _actionNodeList->count();

		for ( int i = 0; i < nodeNum; i++ )
		{
			ActionNode* actionNode = static_cast<ActionNode*>(_actionNodeList->getObjectAtIndex(i));

			if (actionNode->isActionDoneOnce() == false)
			{
				isEnd = false;
				break;
			}
		}

		if (isEnd)
		{
			this->play();
		}

		//CCLOG("ActionObject Update");
	}
}
}