import { ComponentFixture, TestBed } from '@angular/core/testing';

import { RailsViewComponent } from './rails-view.component';

describe('RailsViewComponent', () => {
  let component: RailsViewComponent;
  let fixture: ComponentFixture<RailsViewComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ RailsViewComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(RailsViewComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
