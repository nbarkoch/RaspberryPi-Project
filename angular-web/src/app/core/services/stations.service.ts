import { Injectable } from '@angular/core';
import { BehaviorSubject, Observable } from 'rxjs';
import { Station } from '../models/station.model';
import { HttpClient} from '@angular/common/http';
import { environment } from 'src/environments/environment';
import { Camera } from '../models/camera.model';
import { Alert } from '../models/alert.model';

@Injectable({
  providedIn: 'root'
})
export class StationsService {

  private stationsData$ = new BehaviorSubject<Station[]>([]);
  private firstTimeLoaded = false;
  constructor(private httpClient: HttpClient) {

    //this.loadStations();

    // let stations: Station[] = [{id: 1, city: 'Tel Aviv', name: 'HaHagana'},
    // {id: 2, city: 'Jerusalem', name: 'Itzhak Navon'},
    //  {id: 3, city: 'Tel Aviv', name: 'Ha-Shalom'},
    //  {id: 4, city: 'Be`er Sheva', name: 'Tzafon'},
    //  {id: 5, city: 'Tel Aviv', name: 'Hanna'},
    //  {id: 6, city: 'Tel Aviv', name: 'University'},
    //  {id: 7, city: 'Rishon Letzion', name: 'Moshe Dayan'},
    //  {id: 8, city: 'Rishon Letzion', name: 'Ha-rishonim'},
    //  {id: 9, city: 'Jerusalem', name: 'Malcha'},
    //  {id: 10, city: 'Jerusalem', name: 'Animals Garden'},
    // {id: 11, city: 'Ben Gurion', name: 'Bun Gurion Airport'},
    //  {id: 12, city: 'Be`er Sheva', name: 'Merkaz'}];



    // let cameras: Camera[] = [];
    // for(let i=0;i < 100;i++){
    //   cameras.push({id: i + 1, name: `Camera ${i + 1}`, active: true});
    // }

    // let amount = cameras.length;
    // let j = 0;
    // for(let i=0; i < stations.length;++i){
    //   let current = this.randomIntFromInterval(0, 10);
    //   //j += current;
    //   stations[i].cameras = [];

    //   for(let k = 0;k < current && (j + k) < cameras.length;++k)
    //     stations[i].cameras.push(cameras[j + k]);
    //   j += current;
    //   amount -= current;

    // }

    // let camera1: Camera = {id: 1, name: 'Camera 1', active: true};
    // let camera2: Camera = {id: 2, name: 'Camera 2', active: true};
    // let camera3: Camera = {id: 3, name: 'Camera 3', active: true};
    // let camera4: Camera = {id: 4, name: 'Camera 4', active: true};
    // let camera5: Camera = {id: 5, name: 'Camera 5', active: true};


    //this.stationsData$.next(stations);







   }

   async initStations(){
    if(!this.firstTimeLoaded){
      this.firstTimeLoaded = true;
      await this.loadStations();
    }
  }

    randomIntFromInterval(min, max) { // min and max included
    return Math.floor(Math.random() * (max - min + 1) + min)
  }
  getStations() : Observable<Station[]>{
    return this.stationsData$.asObservable();
  }

  async loadStations(): Promise<void>{
    try{
      let stations = await this.httpClient.get<Station[]>(`${environment.serverURL}get_all_stations_details`).toPromise();
      console.log(stations);
      console.log('loaded data');
      this.stationsData$.next(stations);
    }catch(e){
      console.log('error load stations - ' + e);
    }
  }

  getStationByID(id: string) : Station{
    let stations =  this.stationsData$.value;
    let index = stations.findIndex(x => x.id == id);
    if(index >= 0){
      return stations[index];
    }
    return null;
  }

  AddAlert(stationID: string, cameraID: string, alert: Alert){
    let stations = this.stationsData$.value;
    let index = stations.findIndex(x => x.id == stationID);
    if(index >= 0){
      let cameraIndex = stations[index].cameras.findIndex(x => x.id == cameraID);
      if(cameraIndex >= 0){
        if(stations[index].cameras[cameraIndex].alerts.filter(x => x.id == alert.id).length == 0){
          stations[index].cameras[cameraIndex].alerts.unshift(alert);
          this.stationsData$.next(stations);
        }


      }
    }
  }

  RemoveAlert(stationID: string, cameraID: string, alertID: string){
    let stations = this.stationsData$.value;
    let index = stations.findIndex(x => x.id == stationID);
    if(index >= 0){
      let cameraIndex = stations[index].cameras.findIndex(x => x.id == cameraID);
      if(cameraIndex >= 0){
        let alertIndex = stations[index].cameras[cameraIndex].alerts.findIndex(x => x.id == alertID);
        if(alertIndex >= 0){
          stations[index].cameras[cameraIndex].alerts.splice(alertIndex, 1);
          this.stationsData$.next(stations);
        }
      }
    }
  }

  UpdateAlert(stationID: string, cameraID: string, alert: Alert){
    let stations = this.stationsData$.value;
    let index = stations.findIndex(x => x.id == stationID);
    if(index >= 0){
      let cameraIndex = stations[index].cameras.findIndex(x => x.id == cameraID);
      if(cameraIndex >= 0){
        let alertIndex = stations[index].cameras[cameraIndex].alerts.findIndex(x => x.id == alert.id);
        if(alertIndex >= 0){
          stations[index].cameras[cameraIndex].alerts[alertIndex] = alert;
          this.stationsData$.next(stations);
        }
      }
    }
  }


  UpdateS3UploadAlert(stationID: string, cameraID: string, fileID: string){
    let stations = this.stationsData$.value;
    let index = stations.findIndex(x => x.id == stationID);
    if(index >= 0){
      let cameraIndex = stations[index].cameras.findIndex(x => x.id == cameraID);
      if(cameraIndex >= 0){
        let alertIndex =  stations[index].cameras[cameraIndex].alerts.findIndex(x => x.fileID == fileID);
        if(alertIndex >= 0){
          stations[index].cameras[cameraIndex].alerts[alertIndex].s3Upload = true;
          this.stationsData$.next(stations);
        }
      }
    }
  }





}
