import { Camera } from "./camera.model";

export interface IStation{
  id: string;
  name: string;
  city: string;
  cameras?: Camera[];
};

export type Station = IStation;
