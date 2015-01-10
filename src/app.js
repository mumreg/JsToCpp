
var HelloWorldLayer = cc.Layer.extend({
    sprite:null,
    ctor:function () {
        //////////////////////////////
        // 1. super init first
        this._super();
        var test = new BindingTest();
        test.onFunctionDoneCallback = function() {
			cc.log("this is callback");
		}
        
        test.functionTest();
        test.setText("this is test string");
        cc.log("test.text: ", test.text);
        return true;
    }
});

var HelloWorldScene = cc.Scene.extend({
    onEnter:function () {
        this._super();
        var layer = new HelloWorldLayer();
        this.addChild(layer);
    }
});

