#### 1) Team members (all from section 10):
```
Cuong Ha      – ha000065
Hank Berger   – berg2807
Andy Cedeno   – ceden007
Maxwell Valek – valek016
```

#### 2) Sprint retrospective:
- 04/14/2022: Today we started working on implementing two types of drones. It was a little bit tricky as we did not know where the CreateEntity function was called, how to create the drones automatically, as well as how to send the information of the type of drone chosen by the customer to the back-end. We spent our whole meeting working on but had no result, so we decided to let everyone figure out themselves when they had time.
- 04/16/2022: Today we finally found out that the data contained in `umn.json` would be used to create things as soon as the website was loaded. Therefore, we successfully creating two drones. We then started working on creating a drop box to let the customer choose what type of drone they wanted when they scheduled, and sending that information to the back-end. Thanks to Hank's experience in working with HTML, he managed to add some pieces of code inside the `schedule.html` and it worked.
- 04/18/2022: Today we started working on adding the PrintReceiptDecorator and had the SpinDecorator wrapped by it, so that a receipt would be printed right after the drone finished spinning. We reused the SpinDecorator's code for this new decorator, but then realized we did not have the mean to access to all of the information of the trip just by that. It took us a while, but we managed to figure out that we could actually pass the information into the PrintReceiptDecorator's constructor when we were wrapping SpinDecorator, and printed the information from inside the Move function within PrintReceiptDecorator class.
- 04/20/2022: Today we decided to make our current product a bit more interesting by adding a UFO and let it chase the drone to capture the customer. Thanks to earlier experience with creating and adding entity to the scene, we managed to get it done very quickly. However, we got totally stuck in attempting to implement the movement logic of the UFO. Its logic was very different than the regular drones because it had to update its toTargetPosStrategy frequently to adjust its path accordingly based on the customer's latest position. Once again, we decided to let everyone figure this out themselves in their free time.
- 04/22/2022: Today we still could not figure out how to fix the UFO's movement logic, so we decided to work on AscendingDecorator first. This decorator was used to wrap the UFO's toTargetPosStrategy so that if it successfully captured the customer, it would bring the customer to the sky. This process is quite similar to PrintReceiptDecorator, so it did not take us too long to finish. After we were done with the decorator. we came back to the UFO. This time, we decided to write down some `std::cout` statements to see where a certain if statement the UFO would use and figured out that it was not using correctly. We figured out it was the conditions that caused this problem, but we did know not what exactly was wrong. Therefore, we let everyone figured out themselves.
- 04/24/2022: Today we focused mostly in tackling the logic issue. We wrote down `std::cout` statements all over the place to see why one of the conditions failed. Finally, we found out that the conditions for distances we were using were too small compared to what the actual distances were. We quickly changed those values and finally, the UFO started to move the way we wanted.
- 04/26/2022: Today Hank decided to do some CSS and HTML stuff. He wanted to change the appearance of the schedule page so that it looked a bit more beautiful and outstanding. He also added a feature in the simulation's index where it shows how much the total cost is so far. He did most of the stuff because the others did not really have much experience with those things. Instead they worked on adding information into this `README.md` and Homework 4 Write-up. We ended our meeting sooner than usual to let everyone have time to finish their deadlines for other classes.
- 04/30/2022: Today we met each other for the last time to wrap things up, work on recording video and doing presentation, and finally thanking each other for the last few weeks of working together. Everything went nicely, so we once again ended the meeting sooner than usual.

#### 3) Additional resources for team members:
- Find useful GitHub commands [here](https://github.com/joshnh/Git-Commands), credit to Joshua Hibbert
- Please refer to [this](https://github.umn.edu/umn-csci-3081-s22/shared-upstream/tree/lab09/labs/lab09_git) to review forking and merging
- Please refer to [this](https://stackoverflow.com/questions/7244321/how-do-i-update-or-sync-a-forked-repository-on-github) to learn how to keep your forked repo up-to-date

#### 4) Review of how to run the program:
- Note #1: make sure you are cloning the entire lab09_git_3081w_ha000065 folder, not just the project folder itself
- Note #2: use command `clear` to clear everything showing on your terminal if you want your terminal to look clean before running the program
- Note #3: you can write all the commands at once and separate them with semicolons, it can save you a bit time from typing one by one
- Note #4: in the last step, if you don't like using `localhost`, you can change it to `127.0.0.1`
```
# Clean the project
% make clean

# Build the project
% make -j

# Run the project (./build/web-app <port> <web folder>)
% ./build/bin/transit_service 8081 apps/transit_service/web/

# Open the browser
# Type in localhost:8081 to load the simulation and localhost:8081/schedule.html to load the schedule page
```
