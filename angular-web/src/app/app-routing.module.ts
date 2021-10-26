import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { RailsListComponent } from './shared/components/rails-list/rails-list.component';
import { RailsViewComponent } from './shared/components/rails-view/rails-view.component';

const routes: Routes = [
  {path: 'station/:id', component: RailsViewComponent},
  {path: 'railways', component: RailsListComponent},
  {path: '', redirectTo: 'railways', pathMatch: 'full'},
  {path: '**', redirectTo: 'railways'}
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
