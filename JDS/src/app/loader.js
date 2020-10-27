//滚动加载器，参数为panelbody的id
function Loader(panelbody,msg){
    if (!(this instanceof Loader)){
        return new Loader(panelbody, msg);
    }
    var panelBody=$("#"+panelbody);
    var panel=panelBody.parent('.panel');
    var width=panel.width();
    var height=panel.height()-36;
    var html1='<div class="load-back"></div>';
    this.loadBack=$(html1).css("width",width).css("height",height);
    //this.loadBack.appendTo(panel);
    panel.append(this.loadBack);
    if(!msg){
        msg="处理中..."
    }
    var html2='<div class="load"><img src="/resource/images/loading.gif" style="width:50px;height:50px"/><span>'+msg+'</span></div>';
    this.load=$(html2).css("width",width).css("height",height);
    this.load.appendTo(panel);
}
Loader.prototype.close=function(){
    if(this.loadBack){
        this.loadBack.remove();
        this.loadBack=null;
    }
    if( this.load){
        this.load.remove();
        this.load=null;
    }
}