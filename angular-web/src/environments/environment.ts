// This file can be replaced during build by using the `fileReplacements` array.
// `ng build --prod` replaces `environment.ts` with `environment.prod.ts`.
// The list of file replacements can be found in `angular.json`.

export const environment = {
  production: false,
  serverURL: 'http://localhost:9003/',
  pubnubKeys: {publishkey: 'pub-c-82a0abf0-186c-47ae-8db9-ef683eb3a68c',
              subscribekey: 'sub-c-bd5ecdfe-113b-11ec-b08e-9adb7f1f2877',
              channel: 'detection-channel'},
};

/*
 * For easier debugging in development mode, you can import the following file
 * to ignore zone related error stack frames such as `zone.run`, `zoneDelegate.invokeTask`.
 *
 * This import should be commented out in production mode because it will have a negative impact
 * on performance if an error is thrown.
 */
// import 'zone.js/dist/zone-error';  // Included with Angular CLI.
