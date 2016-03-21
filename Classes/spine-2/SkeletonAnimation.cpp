/******************************************************************************
 * Spine Runtimes Software License
 * Version 2.1
 * 
 * Copyright (c) 2013, Esoteric Software
 * All rights reserved.
 * 
 * You are granted a perpetual, non-exclusive, non-sublicensable and
 * non-transferable license to install, execute and perform the Spine Runtimes
 * Software (the "Software") solely for internal use. Without the written
 * permission of Esoteric Software (typically granted by licensing Spine), you
 * may not (a) modify, translate, adapt or otherwise create derivative works,
 * improvements of the Software or develop new applications using the Software
 * or (b) remove, delete, alter or obscure any trademarks or any copyright,
 * trademark, patent or other intellectual property or proprietary rights
 * notices on or in the Software, including any copy thereof. Redistributions
 * in binary or source form must include this license and terms.
 * 
 * THIS SOFTWARE IS PROVIDED BY ESOTERIC SOFTWARE "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL ESOTERIC SOFTARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include <spine/SkeletonAnimation.h>
#include <spine/spine-cocos2dx.h>
#include <spine/extension.h>
#include <algorithm>

USING_NS_CC;
using std::min;
using std::max;
using std::vector;

namespace spine {

void animationCallback (spAnimationState* state, int trackIndex, spEventType type, spEvent* event, int loopCount) {
	((NEWSkeletonAnimation*)state->rendererObject)->onAnimationStateEvent(trackIndex, type, event, loopCount);
}

void trackEntryCallback (spAnimationState* state, int trackIndex, spEventType type, spEvent* event, int loopCount) {
	((NEWSkeletonAnimation*)state->rendererObject)->onTrackEntryEvent(trackIndex, type, event, loopCount);
}

typedef struct _TrackEntryListeners {
	StartListener startListener;
	EndListener endListener;
	CompleteListener completeListener;
	EventListener eventListener;
} _TrackEntryListeners;

static _TrackEntryListeners* getListeners (spTrackEntry* entry) {
	if (!entry->rendererObject) {
		entry->rendererObject = NEW(spine::_TrackEntryListeners);
		entry->listener = trackEntryCallback;
	}
	return (_TrackEntryListeners*)entry->rendererObject;
}

void disposeTrackEntry (spTrackEntry* entry) {
	if (entry->rendererObject) FREE(entry->rendererObject);
	_spTrackEntry_dispose(entry);
}

//

NEWSkeletonAnimation* NEWSkeletonAnimation::createWithData (spSkeletonData* skeletonData) {
	NEWSkeletonAnimation* node = new NEWSkeletonAnimation(skeletonData);
	node->autorelease();
	return node;
}

NEWSkeletonAnimation* NEWSkeletonAnimation::createWithFile (const char* skeletonDataFile, spAtlas* atlas, float scale) {
	NEWSkeletonAnimation* node = new NEWSkeletonAnimation(skeletonDataFile, atlas, scale);
	node->autorelease();
	return node;
}

NEWSkeletonAnimation* NEWSkeletonAnimation::createWithFile (const char* skeletonDataFile, const char* atlasFile, float scale) {
	NEWSkeletonAnimation* node = new NEWSkeletonAnimation(skeletonDataFile, atlasFile, scale);
	node->autorelease();
	return node;
}

void NEWSkeletonAnimation::initialize () {
	ownsAnimationStateData = true;
	state = spAnimationState_create(spAnimationStateData_create(skeleton->data));
	state->rendererObject = this;
	state->listener = animationCallback;

	_spAnimationState* stateInternal = (_spAnimationState*)state;
	stateInternal->disposeTrackEntry = disposeTrackEntry;
}

NEWSkeletonAnimation::NEWSkeletonAnimation (spSkeletonData *skeletonData)
		: SkeletonRenderer(skeletonData) {
	initialize();
}

NEWSkeletonAnimation::NEWSkeletonAnimation (const char* skeletonDataFile, spAtlas* atlas, float scale)
		: SkeletonRenderer(skeletonDataFile, atlas, scale) {
	initialize();
}

NEWSkeletonAnimation::NEWSkeletonAnimation (const char* skeletonDataFile, const char* atlasFile, float scale)
		: SkeletonRenderer(skeletonDataFile, atlasFile, scale) {
	initialize();
}

NEWSkeletonAnimation::~NEWSkeletonAnimation () {
	if (ownsAnimationStateData) spAnimationStateData_dispose(state->data);
	spAnimationState_dispose(state);
}

void NEWSkeletonAnimation::update (float deltaTime) {
	super::update(deltaTime);

	deltaTime *= timeScale;
	spAnimationState_update(state, deltaTime);
	spAnimationState_apply(state, skeleton);
	spSkeleton_updateWorldTransform(skeleton);
}

void NEWSkeletonAnimation::setAnimationStateData (spAnimationStateData* stateData) {
	CCASSERT(stateData, "stateData cannot be null.");

	if (ownsAnimationStateData) spAnimationStateData_dispose(state->data);
	spAnimationState_dispose(state);

	ownsAnimationStateData = false;
	state = spAnimationState_create(stateData);
	state->rendererObject = this;
	state->listener = animationCallback;
}

void NEWSkeletonAnimation::setMix (const char* fromAnimation, const char* toAnimation, float duration) {
	spAnimationStateData_setMixByName(state->data, fromAnimation, toAnimation, duration);
}

spTrackEntry* NEWSkeletonAnimation::setAnimation (int trackIndex, const char* name, bool loop) {
	spAnimation* animation = spSkeletonData_findAnimation(skeleton->data, name);
	if (!animation) {
		log("Spine: Animation not found: %s", name);
		return 0;
	}
	return spAnimationState_setAnimation(state, trackIndex, animation, loop);
}

spTrackEntry* NEWSkeletonAnimation::addAnimation (int trackIndex, const char* name, bool loop, float delay) {
	spAnimation* animation = spSkeletonData_findAnimation(skeleton->data, name);
	if (!animation) {
		log("Spine: Animation not found: %s", name);
		return 0;
	}
	return spAnimationState_addAnimation(state, trackIndex, animation, loop, delay);
}

spTrackEntry* NEWSkeletonAnimation::getCurrent (int trackIndex) { 
	return spAnimationState_getCurrent(state, trackIndex);
}

void NEWSkeletonAnimation::clearTracks () {
	spAnimationState_clearTracks(state);
}

void NEWSkeletonAnimation::clearTrack (int trackIndex) {
	spAnimationState_clearTrack(state, trackIndex);
}

void NEWSkeletonAnimation::onAnimationStateEvent (int trackIndex, spEventType type, spEvent* event, int loopCount) {
	switch (type) {
	case SP_ANIMATION_START:
		if (startListener) startListener(trackIndex);
		break;
	case SP_ANIMATION_END:
		if (endListener) endListener(trackIndex);
		break;
	case SP_ANIMATION_COMPLETE:
		if (completeListener) completeListener(trackIndex, loopCount);
		break;
	case SP_ANIMATION_EVENT:
		if (eventListener) eventListener(trackIndex, event);
		break;
	}
}

void NEWSkeletonAnimation::onTrackEntryEvent (int trackIndex, spEventType type, spEvent* event, int loopCount) {
	spTrackEntry* entry = spAnimationState_getCurrent(state, trackIndex);
	if (!entry->rendererObject) return;
	_TrackEntryListeners* listeners = (_TrackEntryListeners*)entry->rendererObject;
	switch (type) {
	case SP_ANIMATION_START:
		if (listeners->startListener) listeners->startListener(trackIndex);
		break;
	case SP_ANIMATION_END:
		if (listeners->endListener) listeners->endListener(trackIndex);
		break;
	case SP_ANIMATION_COMPLETE:
		if (listeners->completeListener) listeners->completeListener(trackIndex, loopCount);
		break;
	case SP_ANIMATION_EVENT:
		if (listeners->eventListener) listeners->eventListener(trackIndex, event);
		break;
	}
}

void NEWSkeletonAnimation::setStartListener (spTrackEntry* entry, StartListener listener) {
	getListeners(entry)->startListener = listener;
}

void NEWSkeletonAnimation::setEndListener (spTrackEntry* entry, EndListener listener) {
	getListeners(entry)->endListener = listener;
}

void NEWSkeletonAnimation::setCompleteListener (spTrackEntry* entry, CompleteListener listener) {
	getListeners(entry)->completeListener = listener;
}

void NEWSkeletonAnimation::setEventListener (spTrackEntry* entry, spine::EventListener listener) {
	getListeners(entry)->eventListener = listener;
}

}
