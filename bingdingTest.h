//
//  bingdingTest.h
//  cocos2d_js_bindings
//
//  Created by Mikhail Perekhodtsev on 10.01.15.
//
//

#ifndef __cocos2d_js_bindings__bingdingTest__
#define __cocos2d_js_bindings__bingdingTest__

#include "jsapi.h"
#include "jsfriendapi.h"
#include "js_bindings_config.h"
#include "ScriptingCore.h"
#include "jsb_helper.h"

class BindingTest : public cocos2d::Ref {
public:
    BindingTest();
    ~BindingTest();
    
    JS_BINDED_CLASS_GLUE(BindingTest);
    JS_BINDED_CONSTRUCTOR(BindingTest);
    
    JS_BINDED_FUNC(BindingTest, functionTest); // call empty method
    JS_BINDED_FUNC(BindingTest, setText); // call method with argument/set property
    
    JS_BINDED_PROP_ACCESSOR(BindingTest, onFunctionDoneCallback); //call js method from c++
    JS_BINDED_PROP_GET(BindingTest, text); //get property
    
private:
    void _functionTest(JSContext *cx);
    void _setText(const std::string &newText);
    JSContext* _cx;
    JSObject*  _onFunctionDoneCallback;
    std::string _text;
};

#endif /* defined(__cocos2d_js_bindings__bingdingTest__) */
