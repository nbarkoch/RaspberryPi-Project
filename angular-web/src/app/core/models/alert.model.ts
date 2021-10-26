export interface IAlert{
  id: string;
  cameraID: string;
  // imageURL: string;
  // videoURL: string;
  fileID: string;
  s3Upload: boolean;
  seen: boolean;
  registeredDate: Date;
};

export type Alert = IAlert;
