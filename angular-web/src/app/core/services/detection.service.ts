import { Injectable } from '@angular/core';
import { PubNubAngular } from 'pubnub-angular2';
import { environment } from 'src/environments/environment';
import { StationsService } from './stations.service';

@Injectable({
  providedIn: 'root'
})
export class DetectionService {
  // private publishkey = 'pub-c-6662ea57-6bf8-4f52-88f8-8cc4aa36f23a';
  // private subscribekey = 'sub-c-5c8eb4aa-b1da-11eb-b48e-0ae489c2794e';
  // private channel = 'detection-channel';
  isListening = false;
  constructor(private pubnub: PubNubAngular, private stationsService: StationsService) {
    if(!this.isListening)
      this.pubnubInit();
    this.isListening = true;
   }


 async  pubnubInit() {

    await this.stationsService.initStations();

    this.pubnub.init({
      publishKey: environment.pubnubKeys.publishkey,
      subscribeKey: environment.pubnubKeys.subscribekey,
      ssl: true,
      uuid: "client"
    });



      this.pubnub.addListener({
        status: function (st) {

            // this.pubnub.publish({
            //   message: {sender: "Matan",text: 'asdfasf',id: '200'},
            //   channel: 'backend-session'
            // });

        },
        message: (message: any) => {
          console.log('got message: ');
          console.log(message);
          let alertData = message.message;
          if(alertData.type == '1'){
            this.stationsService.AddAlert(alertData.stationID, alertData.cameraID, alertData.alert);
          }
          else if(alertData.type == '2'){
            this.stationsService.UpdateS3UploadAlert(alertData.stationID, alertData.cameraID, alertData.fileID);
          }
          //this.handleCommand(channelCommand.message);
          //this.push({id: 1, username: message.message.sender, message: message.message.text, date: null})
          //this.msg= message.message.content;
          //console.log(this.msg);
        }
      });


      //var channelsList = this.userChannels.value.map(c => c.identifier);
      this.pubnub.subscribe({
      channels: [environment.pubnubKeys.channel],//['backend-session'],
      withPresence: true,
      triggerEvents: ['message', 'presence', 'status']
    });
  }

   async  publishCommand(messageText: string){
    await this.pubnub.publish({
      message: messageText,//{sender: this.userName,text: this.myText,id: '200'},
      channel: environment.pubnubKeys.channel
    }).catch(error => {  // catch the errors
      console.log(error);
    });;

    //this.myText = "";
   }
}
