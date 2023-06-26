var ev = {
    "Level_A":{ type: "Level_A", text: "leq" , value: "" , warningLimit: 3, dangerLimit:3.2 , unity:"" , vibUnity:"" , format:"f" , showResult:false},
    "H":{ type: "H", text: "频率复杂度" , value: "" , warningLimit: 3, dangerLimit:3.2 , unity:"" , vibUnity:"" , format:"f" , showResult:true},
    "fm":{ type: "fm", text: "主频" , value: "" , warningLimit: 399, dangerLimit:400 , unity:"Hz" , vibUnity:"Hz" , format:"d" , showResult:false},
    "pm":{ type: "pm", text: "主频占比" , value: "" , warningLimit: 0.65, dangerLimit:0.79, unity:"", vibUnity:""  , format:"f" , showResult:true},
    "rms":{ type: "rms", text: "均方根" ,value:"", warningLimit: 1, dangerLimit:5 , unity:"Pa", vibUnity:"g"  , format:"f" , showResult:true },
    "p_even":{ type: "p_even", text: "偶次比" , value: "", warningLimit: 1, dangerLimit:5 , unity:"", vibUnity:"" , format:"f" , showResult:true},
    "p_odd":{ type: "p_odd", text: "奇次比" , value:"", warningLimit: 1, dangerLimit:5 , unity:"" , vibUnity:"" , format:"f", showResult:true},
    "pf":{ type: "pf", text: "基频占比" , value:"" , warningLimit: 1, dangerLimit:5, unity:"" , vibUnity:"", format:"f", showResult:true},
    "spFreRatio":{ type: "spFreRatio", text: "频段能量比" , value:"", warningLimit: 1, dangerLimit:5, unity:"" , vibUnity:"", format:"f" , showResult:true},

    "EDR":{ type: "EDR", text: "能量差异度" , value:"", warningLimit: 1, dangerLimit:5, unity:"" , vibUnity:"", format:"f" , showResult:true},
    "spCentroid":{ type: "spCentroid", text: "频率中心" , value:"", warningLimit: 1, dangerLimit:5, unity:"Hz" , vibUnity:"Hz", format:"f" , showResult:true},
    "spCrest":{ type: "spCrest", text: "频率峰度" , value:"", warningLimit: 1, dangerLimit:5, unity:"" , vibUnity:"", format:"f" , showResult:true},
    "spDecrease":{ type: "spDecrease", text: "频率衰减" , value:"", warningLimit: 1, dangerLimit:5, unity:"" , vibUnity:"", format:"f" , showResult:true},
    "spFlatness":{ type: "spFlatness", text: "频率平坦度" , value:"", warningLimit: 1, dangerLimit:5, unity:"" , vibUnity:"", format:"f" , showResult:true},
    "spEntropy":{ type: "spEntropy", text: "谱熵" , value:"", warningLimit: 1, dangerLimit:5, unity:"" , vibUnity:"", format:"f" , showResult:true},
    "spKurtosis":{ type: "spKurtosis", text: "谱峭度" , value:"", warningLimit: 1, dangerLimit:5, unity:"" , vibUnity:"", format:"f" , showResult:true},
    "spSkewness":{ type: "spSkewness", text: "谱偏度" , value:"", warningLimit: 1, dangerLimit:5, unity:"" , vibUnity:"", format:"f" , showResult:true},
    "spSlope":{ type: "spSlope", text: "谱斜率" , value:"", warningLimit: 1, dangerLimit:5, unity:"" , vibUnity:"", format:"f" , showResult:true},
    "spSpread":{ type: "spSpread", text: "扩频" , value:"", warningLimit: 1, dangerLimit:5, unity:"" , vibUnity:"", format:"f" , showResult:true},
    "spRolloff":{ type: "spRolloff", text: "95%能量点" , value:"", warningLimit: 1, dangerLimit:5, unity:"Hz" , vibUnity:"Hz", format:"f" , showResult:true},
    "fmh":{ type: "fmh", text: "最大谐波频率" , value:"", warningLimit: 1, dangerLimit:5, unity:"Hz" , vibUnity:"Hz", format:"f" , showResult:true}

}

function getEvList( arr ){
    if( Array.isArray( arr ) ){
        var res = [];
        for( var i = 0; i < arr.length; i++ ){
            res[i] = ev[arr[i]];
        }
        return res;
    }else{
        return[];
    }
}
