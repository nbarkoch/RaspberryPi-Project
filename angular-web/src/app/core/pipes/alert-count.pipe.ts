import { Pipe, PipeTransform } from '@angular/core';
import { Camera } from '../models/camera.model';
import { Station } from '../models/station.model';

@Pipe({
  name: 'alertCount',
  pure: false
})
export class AlertCountPipe implements PipeTransform {

  transform(stationOrCamera: Station | Camera, ...args: unknown[]): number {

    if((stationOrCamera as Station).cameras !== undefined) {
      return (stationOrCamera as Station).cameras.reduce((acc, camera) => { return acc + camera.alerts.filter(a => a.seen == false).length;}, 0);
    }
    else{
      return (stationOrCamera as Camera).alerts.filter(a => a.seen == false).length;
    }

    // let notifictionsCount = 0;
    // for(let camera of stationOrCamera?.cameras){
    //   notifictionsCount += camera.alerts.filter(a => a.seen == false).length; //camera.notificationCount ?  camera.notificationCount : 0 ;
    // }
    // return notifictionsCount;
  }

}
