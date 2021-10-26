import { ComponentFixture, TestBed } from '@angular/core/testing';

import { RailsListComponent } from './rails-list.component';

describe('RailsListComponent', () => {
  let component: RailsListComponent;
  let fixture: ComponentFixture<RailsListComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ RailsListComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(RailsListComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
