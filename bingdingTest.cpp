//
//  bingdingTest.cpp
//  cocos2d_js_bindings
//
//  Created by Mikhail Perekhodtsev on 10.01.15.
//
//

#include "bingdingTest.h"
#include "cocos2d_specifics.hpp"

BindingTest::BindingTest()
:_cx(ScriptingCore::getInstance()->getGlobalContext())
,_text()
,_onFunctionDoneCallback(nullptr)
{
}

/**
 * @brief Destructor cleans up _httpRequest and stuff
 *
 */
BindingTest::~BindingTest()
{
}

JS_BINDED_CLASS_GLUE_IMPL(BindingTest);

/**
 *  @brief Implementation for the Javascript Constructor
 *
 */
JS_BINDED_CONSTRUCTOR_IMPL(BindingTest)
{
    BindingTest* req = new BindingTest();
    req->autorelease();
    
    js_proxy_t *p;
    jsval out;
    
    JSObject *obj = JS_NewObject(cx, &BindingTest::js_class, BindingTest::js_proto, BindingTest::js_parent);
    
    if (obj) {
        JS_SetPrivate(obj, req);
        out = OBJECT_TO_JSVAL(obj);
    }
    
    JS_SET_RVAL(cx, vp, out);
    p =jsb_new_proxy(req, obj);
    
    JS_AddNamedObjectRoot(cx, &p->obj, "BindingTest");
    return true;
}

JS_BINDED_FUNC_IMPL(BindingTest, functionTest)
{
    _functionTest(cx);
    return true;
}

void BindingTest::_functionTest(JSContext *cx)
{
    cocos2d::log("Binding test");
    js_proxy_t *p;
    void *ptr = (void *)this;
    p = jsb_get_native_proxy(ptr);
    if (p) {
        if (_onFunctionDoneCallback) {
            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
            jsval fval = OBJECT_TO_JSVAL(_onFunctionDoneCallback);
            jsval out;
            JS_CallFunctionValue(cx, NULL, fval, 0, NULL, &out);
        }
    }
}

JS_BINDED_PROP_GET_IMPL(BindingTest, onFunctionDoneCallback)
{
    if (_onFunctionDoneCallback) {
        JSString *tmpstr = JS_NewStringCopyZ(cx, "1");
        JS::RootedValue tmpval(cx);
        tmpval = STRING_TO_JSVAL(tmpstr);
        JS_SetProperty(cx, _onFunctionDoneCallback, "functionTest", tmpval);
        
        jsval out = OBJECT_TO_JSVAL(_onFunctionDoneCallback);
        vp.set(out);
    } else {
        vp.set(JSVAL_NULL);
    }
    return true;
}

JS_BINDED_PROP_SET_IMPL(BindingTest, onFunctionDoneCallback)
{
    jsval callback = vp.get();
    if (callback != JSVAL_NULL) {
        _onFunctionDoneCallback = JSVAL_TO_OBJECT(callback);
        JS_AddNamedObjectRoot(cx, &_onFunctionDoneCallback, "onFunctionDoneCallback");
    }
    return true;
}

JS_BINDED_PROP_GET_IMPL(BindingTest, text)
{
    jsval strVal = std_string_to_jsval(cx, _text);
    
    if (strVal != JSVAL_NULL)
    {
        vp.set(strVal);
        return true;
    }
    else
    {
        JS_ReportError(cx, "Error trying to create JSString from data");
        return false;
    }
}

JS_BINDED_FUNC_IMPL(BindingTest, setText)
{
    if (argc >= 1) {
        jsval* argv = JS_ARGV(cx, vp);
        const char *text;
        
        JSString* jsText = JS::ToString( cx, JS::RootedValue(cx, argv[0]) );
        JSStringWrapper w(jsText);
        text = w.get();
        _setText(text);
        
        return true;
    }
    
    return false;
}

void BindingTest::_setText(const std::string &newText)
{
    cocos2d::log("BindingTest::_setText to %s", newText.c_str());
    _text = newText;
}

/**
 *  @brief destructor for Javascript
 *
 */
static void basic_object_finalize(JSFreeOp *freeOp, JSObject *obj)
{
    CCLOG("basic_object_finalize %p ...", obj);
}

/**
 *  @brief Register BindingTest to be usable in JS and add properties and Mehtods.
 *  @param cx   Global Spidermonkey JS Context.
 *  @param global   Global Spidermonkey Javascript object.
 */
void BindingTest::_js_register(JSContext *cx, JSObject *global)
{
    JSClass jsclass = {
        "BindingTest", JSCLASS_HAS_PRIVATE, JS_PropertyStub,
        JS_DeletePropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
        JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub,
        basic_object_finalize, JSCLASS_NO_OPTIONAL_MEMBERS
    };
    
    BindingTest::js_class = jsclass;
    static JSPropertySpec props[] = {
        JS_BINDED_PROP_DEF_ACCESSOR(BindingTest, onFunctionDoneCallback),
        JS_BINDED_PROP_DEF_GETTER(BindingTest, text),
        {0, 0, 0, 0, 0}
    };
    
    static JSFunctionSpec funcs[] = {
        JS_BINDED_FUNC_FOR_DEF(BindingTest, functionTest),
        JS_BINDED_FUNC_FOR_DEF(BindingTest, setText),
        JS_FS_END
    };
    
    BindingTest::js_parent = NULL;
    BindingTest::js_proto = JS_InitClass(cx, global, NULL, &BindingTest::js_class , BindingTest::_js_constructor, 0, props, funcs, NULL, NULL);
    
    // add the proto and JSClass to the type->js info hash table
    TypeTest<BindingTest> t;
    js_type_class_t *p;
    std::string typeName = t.s_name();
    if (_js_global_type_map.find(typeName) == _js_global_type_map.end())
    {
        p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
        p->jsclass = &BindingTest::js_class;
        p->proto = BindingTest::js_proto;
        p->parentProto = NULL;
        _js_global_type_map.insert(std::make_pair(typeName, p));
    }
}

