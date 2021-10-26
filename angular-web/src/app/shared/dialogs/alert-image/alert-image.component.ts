import { Component, Inject, OnInit } from '@angular/core';
import { MAT_DIALOG_DATA } from '@angular/material/dialog';
import { DialogData } from '../../components/camera-view/camera-view.component';

@Component({
  selector: 'app-alert-image',
  templateUrl: './alert-image.component.html',
  styleUrls: ['./alert-image.component.scss']
})
export class AlertImageComponent implements OnInit {

  constructor(@Inject(MAT_DIALOG_DATA) public data: DialogData) { }

  ngOnInit(): void {
  }

}
