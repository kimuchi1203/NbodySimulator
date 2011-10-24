package com.NbodySimulator;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.view.View;

import com.NbodySimulator.SimDataModel;

public class SimView extends View {
	private SimDataModel data;
	private int footprintCount=0;
	private int footprintCount2=0;
	public SimView(Context context, SimDataModel dataModel) {
		super(context);
		data = dataModel;
	}
	public void onDraw(Canvas canvas){
		Paint paint = new Paint();
		paint.setAntiAlias(true);
		int i;
		for(i=0;i<data.getNum_p();++i){
			paint.setColor(Color.parseColor(data.getColor()[i]));
			canvas.drawCircle((float)data.getX3()[i*3+0],(float)data.getX3()[i*3+1],(float)Math.log10(data.getMass()[i]+10.0f)*10.0f, paint);
			if(data.isShowFootprint()){
				canvas.drawPoints(data.getFootprint(),i*data.getLen_footprint()*2,footprintCount*2, paint);
			}
			if(footprintCount2%data.getIntervalFootprint()==0){
				//update footprint
				data.getFootprint()[i*data.getLen_footprint()*2+data.getIndexFootprint()*2+0] = (float)data.getX3()[i*3+0];
				data.getFootprint()[i*data.getLen_footprint()*2+data.getIndexFootprint()*2+1] = (float)data.getX3()[i*3+1];
			}
		}
		if(footprintCount2%data.getIntervalFootprint()==0){
			data.inclementIndexFootprint();
			footprintCount2 = 1;
		}else{
			++footprintCount2;
		}
		if(footprintCount<data.getLen_footprint()){
			++footprintCount;
		}
	}
}
