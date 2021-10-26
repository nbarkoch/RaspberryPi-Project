import { Alert } from "./alert.model";

export interface ICamera{
  id: string;
  name: string;
  active: boolean;
  stationID: string;
  alerts: Alert[];
  /*notificationCount?: number;*/
};

export type Camera = ICamera;
