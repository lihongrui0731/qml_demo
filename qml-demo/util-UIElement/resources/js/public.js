function delDataFromArr( arr , d ) {
   var new_arr = []
   for( var i = 0; i < arr.length; i++ ){
        if( arr[i] !== d ){
            new_arr.push( arr[i] )
        }
   }
   return new_arr;
}

function addDataToArr( arr , d ) {
   if( arr.indexOf( d ) == -1 ){
        arr.push(d);
   }
   return arr;
}

function checkNumberRange( value , min , max ){
    if( value >= max && max >= min ){
        value = max
    }
    if( value <= min && min <= max ){
        value = min
    }
    return value;
    // return Math.round(value);
}

function checkNumber( value ){
    value = value.replace(/[^0-9|.]/ig,"");
    return value;
}

function getGlobalPosition(targetObject) {
    var positionX = 0
    var positionY = 0
    var obj = targetObject        /* 遍历所有的父窗口 */
    while (obj != null) {
        /* 累加计算坐标 */
        positionX += obj.x
        positionY += obj.y
        obj = obj.parent
    }
    return {"x": positionX, "y": positionY}
}

function getColorFromBar( value , min , max ){
    if (value < min)
        value = min;
      else if (value > max)
        value = max;
      const dv = max - min;
      const c = {r: 0, g: 0, b: 0};
      const v = (value - min) / dv;
      const _8th = 0.125;

      if (v < _8th) {
        c.b = 128 * (1 + v / _8th);
      } else if (v < 3 * _8th) {
        c.b = 255;
        c.g = 128 * (v - _8th) / _8th;
      } else if (v < 5 * _8th) {
        c.r = 128 * (v - 3 * _8th) / _8th;
        c.g = 255;
        c.b = 128 * (5 * _8th - v) / _8th;
      } else if (v < 7 * _8th) {
        c.r = 255;
        c.g = 128 * (7 * _8th - v) / _8th;
      } else {
        c.r = 128 * (9 * _8th - v) / _8th;
      }
      return c;
}

function scienceNumber (val , format){
    if( val === 0 ){
        return String(val);
    }

    var v = Number(val);
    if( format === "d" ){
        return String(v.toFixed(0));
    }

    var p = Math.floor(Math.log(v)/Math.LN10);
    if( p > -3 ){
        return String( Number(val).toFixed(3) )
    }
    return Number(val).toExponential(3);

};


function formatTimer( time ){
    var string = "00:00:00";
    var h = Math.floor(time / 3600);
    var min = Math.floor( time % 3600 / 60 );
    var s = time % 3600 % 60;

    string = String(h).padStart(2,'0') + ":" + String(min).padStart(2,'0')  + ":" + String(s).padStart(2,'0');

    return string;
}

function formatSize(fileSize){
    if( fileSize / 1024 / 1024 / 1024 > 1 ){
        return (fileSize / 1024 / 1024 / 1024 ).toFixed(1) + "GB"
    }

    if( fileSize / 1024 / 1024 > 1 ){
        return Math.ceil(fileSize / 1024 / 1024 ) + "MB"
    }
    if(  fileSize / 1024 > 1 ){
        return Math.ceil(fileSize / 1024 ) + "KB"
    }
    return (fileSize) + "B"
}

function sortJsonData( json , column ){
    var temp;
    for( let i = 0; i < json.length; i++ ){
        for( let j = i; j < json.length; j++ ){
            if( json[j][column] > json[i][column] ){
                temp = json[j];
                json[j] = json[i];
                json[i] = temp;
            }
        }
    }

    return json;
}

