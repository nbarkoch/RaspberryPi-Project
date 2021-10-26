import { Component, Input, OnInit } from '@angular/core';
import { ActivatedRoute, Router } from '@angular/router';
import { Station } from 'src/app/core/models/station.model';
import { StationsService } from 'src/app/core/services/stations.service';


@Component({
  selector: 'app-rails-view',
  templateUrl: './rails-view.component.html',
  styleUrls: ['./rails-view.component.scss']
})
export class RailsViewComponent implements OnInit {

  expanded: boolean = false;
  @Input() station: Station = null;



   constructor(private activatedRoute: ActivatedRoute,
            private stationsService: StationsService,
            private router: Router) { }

  async ngOnInit(): Promise<void> {

    await this.stationsService.initStations();

    this.activatedRoute.params.subscribe(p => {
      if(p?.id){
        this.expanded = true;
        this.station = this.stationsService.getStationByID(p.id);
        //console.log(this.station);
        console.log('id found:' + p.id);

      }

    })

    // this.stationsService.getStations().subscribe(x => {
    //   let index = x.findIndex(s => s.id == this.station?.id);
    //   if(index >= 0){
    //     this.station = x[index];
    //     console.log(this.station)
    //   }
    // })

  }

  ShowMore(){
    this.router.navigate(['station', this.station?.id]);
  }

}
