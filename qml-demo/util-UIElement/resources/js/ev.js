var ev = {
    "Level_A":{ type: "Level_A", text: "leq" , value: "" , warningLimit: 3, dangerLimit:3.2 , unity:"" , vibUnity:"" , format:"f" , showResult:false},
    "H":{ type: "H", text: "复杂波度" , value: "" , warningLimit: 3, dangerLimit:3.2 , unity:"" , vibUnity:"" , format:"f" , showResult:true},
    "fm":{ type: "fm", text: "峰值频率" , value: "" , warningLimit: 399, dangerLimit:400 , unity:"Hz" , vibUnity:"Hz" , format:"d" , showResult:false},
    "pm":{ type: "pm", text: "峰值系数" , value: "" , warningLimit: 0.65, dangerLimit:0.79, unity:"", vibUnity:""  , format:"f" , showResult:true},
    "rms":{ type: "rms", text: "均方能量" ,value:"", warningLimit: 1, dangerLimit:5 , unity:"Pa", vibUnity:"g"  , format:"f" , showResult:true },
    "p_even":{ type: "p_even", text: "偶波系数" , value: "", warningLimit: 1, dangerLimit:5 , unity:"", vibUnity:"" , format:"f" , showResult:true},
    "p_odd":{ type: "p_odd", text: "奇波系数" , value:"", warningLimit: 1, dangerLimit:5 , unity:"" , vibUnity:"" , format:"f", showResult:true},
    "pf":{ type: "pf", text: "基波系数" , value:"" , warningLimit: 1, dangerLimit:5, unity:"" , vibUnity:"", format:"f", showResult:true},
    "spFreRatio":{ type: "spFreRatio", text: "能比系数" , value:"", warningLimit: 1, dangerLimit:5, unity:"" , vibUnity:"", format:"f" , showResult:true},

    "EDR":{ type: "EDR", text: "能量差值" , value:"", warningLimit: 1, dangerLimit:5, unity:"" , vibUnity:"", format:"f" , showResult:true},
    "spCentroid":{ type: "spCentroid", text: "中心系数" , value:"", warningLimit: 1, dangerLimit:5, unity:"Hz" , vibUnity:"Hz", format:"f" , showResult:true},
    "spCrest":{ type: "spCrest", text: "峰态系数" , value:"", warningLimit: 1, dangerLimit:5, unity:"" , vibUnity:"", format:"f" , showResult:true},
    "spDecrease":{ type: "spDecrease", text: "衰减系数" , value:"", warningLimit: 1, dangerLimit:5, unity:"" , vibUnity:"", format:"f" , showResult:true},
    "spFlatness":{ type: "spFlatness", text: "平坦系数" , value:"", warningLimit: 1, dangerLimit:5, unity:"" , vibUnity:"", format:"f" , showResult:true},
    "spEntropy":{ type: "spEntropy", text: "混度系数" , value:"", warningLimit: 1, dangerLimit:5, unity:"" , vibUnity:"", format:"f" , showResult:true},
    "spKurtosis":{ type: "spKurtosis", text: "峰度系数" , value:"", warningLimit: 1, dangerLimit:5, unity:"" , vibUnity:"", format:"f" , showResult:true},
    "spSkewness":{ type: "spSkewness", text: "偏度系数" , value:"", warningLimit: 1, dangerLimit:5, unity:"" , vibUnity:"", format:"f" , showResult:true},
    "spSlope":{ type: "spSlope", text: "斜率系数" , value:"", warningLimit: 1, dangerLimit:5, unity:"" , vibUnity:"", format:"f" , showResult:true},
    "spSpread":{ type: "spSpread", text: "扩频系数" , value:"", warningLimit: 1, dangerLimit:5, unity:"" , vibUnity:"", format:"f" , showResult:true},
    "spRolloff":{ type: "spRolloff", text: "能点频率" , value:"", warningLimit: 1, dangerLimit:5, unity:"Hz" , vibUnity:"Hz", format:"f" , showResult:true},
    "fmh":{ type: "fmh", text: "整峰值谐波" , value:"", warningLimit: 1, dangerLimit:5, unity:"Hz" , vibUnity:"Hz", format:"f" , showResult:true}

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
