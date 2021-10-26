import { Component, OnInit } from '@angular/core';
import { Station } from 'src/app/core/models/station.model';
import { DetectionService } from 'src/app/core/services/detection.service';
import { StationsService } from 'src/app/core/services/stations.service';
import { AlertCountPipe } from 'src/app/core/pipes/alert-count.pipe';

@Component({
  selector: 'app-rails-list',
  templateUrl: './rails-list.component.html',
  styleUrls: ['./rails-list.component.scss'],
  providers: [AlertCountPipe]
})
export class RailsListComponent implements OnInit {


  stations: Station[] = [];

  constructor(
    private detectionService: DetectionService,
    private stationsService: StationsService) { }

  async ngOnInit(): Promise<void> {
    await this.stationsService.initStations();


    this.stationsService.getStations().subscribe(s => {
      this.stations = [...s];
      //console.log(s);
      this.stations.sort(this.compareNotifications);
      //console.log(this.stations);
    });
  }

  compareNotifications( stationA: Station, stationB: Station ) {
    let p = new AlertCountPipe;
    let notifyCountA = p.transform(stationA);
    let notifyCountB = p.transform(stationB);
    if ( notifyCountA < notifyCountB ){
      return 1;
    }
    if ( notifyCountA > notifyCountB ){
      return -1;
    }
    return stationA.city.localeCompare(stationB.city);
  }

  SendMessage(){
    this.detectionService.publishCommand('This is a test');
  }

}
