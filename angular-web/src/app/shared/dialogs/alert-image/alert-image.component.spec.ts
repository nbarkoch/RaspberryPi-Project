import { ComponentFixture, TestBed } from '@angular/core/testing';

import { AlertImageComponent } from './alert-image.component';

describe('AlertImageComponent', () => {
  let component: AlertImageComponent;
  let fixture: ComponentFixture<AlertImageComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ AlertImageComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(AlertImageComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
