import { Component, Input, OnInit } from '@angular/core';
import { Camera } from 'src/app/core/models/camera.model';
import {MatDialog, MAT_DIALOG_DATA} from '@angular/material/dialog';
import { AlertImageComponent } from '../../dialogs/alert-image/alert-image.component';
import { Alert } from 'src/app/core/models/alert.model';
import { Station } from 'src/app/core/models/station.model';
import { StationsService } from 'src/app/core/services/stations.service';
import { DetectionService } from 'src/app/core/services/detection.service';

export interface DialogData {
  station: Station;
  camera: Camera;
  alertIndex: number;
  baseUrl: string;
}

@Component({
  selector: 'app-camera-view',
  templateUrl: './camera-view.component.html',
  styleUrls: ['./camera-view.component.scss']
})
export class CameraViewComponent implements OnInit {

  baseURL: string = '';
  selectedAlert = -1;
  station: Station = null;
  @Input() camera: Camera = null;
  @Input() showMore: boolean = false;
  constructor(public dialog: MatDialog, private stationsService: StationsService, private detectionService: DetectionService) { }


  openDialog() {
    this.dialog.open(AlertImageComponent, {
      width: '100%',
      minHeight: 'calc(100vh - 90px)',
      height : 'auto',
      data: {
        station: this.station,
        camera: this.camera,
        alertIndex: this.selectedAlert,
        baseUrl: this.baseURL
      }
    });
  }


  ngOnInit(): void {

    if (this.camera) {
      this.station = this.stationsService.getStationByID(this.camera.stationID);
      this.baseURL = `https://cross-rails-detection.s3.us-east-2.amazonaws.com/${this.station.id}/${this.camera.id}/`;
    }


  }

}
