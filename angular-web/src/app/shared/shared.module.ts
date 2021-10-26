import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { RailsListComponent } from './components/rails-list/rails-list.component';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { MaterialModule } from '../core/modules/material/material.module';
import { RailsViewComponent } from './components/rails-view/rails-view.component';
import { CameraViewComponent } from './components/camera-view/camera-view.component';
import { CoreModule } from '../core/core.module';
import { AlertImageComponent } from './dialogs/alert-image/alert-image.component';
//import { PubNubAngular } from 'pubnub-angular2';



@NgModule({
  declarations: [RailsListComponent, RailsViewComponent, CameraViewComponent, AlertImageComponent],
  imports: [
    CommonModule,
    MaterialModule,
    BrowserAnimationsModule,
    CoreModule
  ],
  exports: [RailsListComponent, RailsViewComponent, CameraViewComponent, AlertImageComponent]
})
export class SharedModule { }
