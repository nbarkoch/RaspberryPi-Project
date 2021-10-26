import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { AlertCountPipe } from './pipes/alert-count.pipe';
//import { PubNubAngular } from 'pubnub-angular2';



@NgModule({
  declarations: [AlertCountPipe],
  imports: [
    CommonModule,
  ],
  exports: [AlertCountPipe]

})
export class CoreModule { }
