Contributing
------------

For those who have a basic fix, like a typo correction or simple bug fix, feel free to create Pull requests with those fixes and suggested improvements only after you have tested them. Explain why you are making the request in the comment section of the request.  Explain any testing you did.

The philosophical approach taken with this code is: write well, write succinctly, try to write legible, readible and extensible code smartly, then test extensively, expand additively, remain backward compatible when necessary.  We do not really want to "remove" functionality unless it is truly worthless.  We never want to remove functionality that might be useful even in the smallest bit.  Usefulness is considered a non-zero value when it has been used at least once in one derived project somewhere.

## On the subject of backward compatability during expansion of functionality

If the direction changes for a feature, it should be created as a new component, rather than evolving an existing component as dependencies may be in projects.  Use of method parameterization variation is one way of maintaining backward compatibility.  This can be seen throughout the source code already.  That is why you may see 3 or 4 different approaches to more complicated sets of features.  UI comes to mind as one feature that will never be "resolved as complete", partly due to the fact that there are many ways one can approach a UI, for different reasons and ultimately, different outcomes.

#### As an example for how this process and way of thinking manifests itself in vivo, the UI anecdote goes something like this:

a) I wanted an expandable, smartly written, object oriented approach, so I created Proce55or class.  This was a good way to create an extensible UI framework, and much time was spent on it.  After a few years, however, the cumbersome aspects of it only solved one set of problems.  It took a long time to create, edit and debug, then customize and finally lock-in a feature using this method, though the results were generally good.

b) There was no fast way to create input panels for in-game editors, so I created FastGUI which relied on a functional approach, captured in the FastGUI class.  While this is very useful for creating more complex dialogs, it also was cumbersome mainly when it came to layouts of complex dialogs. Because you had to hand-code each location on the screen, add responsive behavior manually, and spend a lot of time either playing around in Photoshop figuring out the virtual display locations, widths, etc., for the features, or you spent a lot of time simply placing the items on the screen, especially when doing highly complex editor panels and windows.  The placement and manipulation of elements (oh jeez, do I have to reorder assets?  for example this takes a very long time with manual layouts) was tedious and frustrating, thus a drawback to this method.

c) Therefore, FastGUI lead to a related feature EditGUI, which uses FastGUI but has some features of the original Proce55or approach, and is tailored to creating windowed, panning GUIs that can either be manually layed out, or layed out in a semi-automatic or fully automatic fashion.

d)  What will the next UI be?  Who knows, but it would have to be a distinct layer, with its own classes and honed for a particular purpose.  Therefore we would not "replace" Proce55or or FastGUI or EditGUI, but, rather, we would create an entirely new UI/GUI class suite.

## Contributing documentation

I have resisted the urge to use Doxygen comments in the code.  If I were to institute a standard for generating granular documentation, it would be Doxygen comments.  However, the code gets lost in the comments if you use comments to document the code.  Therefore, it's probably not going to be the desired approach.

Generally, the code should speak for itself.  You have to read the code to understand what it does, and you have to debug your application to discover why something didn't work as it did.  

## How this code is commented

Adding justification is fine, especially around complicated technical features that rely on standards not created by the application.  I've gone as far as reproducing the original relevant helpful content (rather than just a link) because of the dynamic nature of today's digital webosphere.  For this reason, you may find papers, articles appearing as comment notices, including originating likes links such as to StackOverflow, including brief credits to the authors on sites like this, mixed into some files.  It's important to include ALL of the justification, not just a reference to it, because you need to know later on if you made the right decision, and often who it was you trusted as your source.

I call single word, single line or in-line comments a "brain nudge" -- just something to explain something that may not initially appear one way, and instead may appear another way which would lead a hurried developer down the wrong path.  Especially when you return to code and are confused multiple times, it is good to have these brief "nudges"

## Naming conventions, specific notes on commenting, etc.

#### I recommend that you follow this pattern:

* If you reach a "place of ambiguity" in code, you should leave a comment to nudge the brain. The comment can be one ``/*word*/``,
or 
```
// <- (optional) an aside on the same line
// Or a sentence describing this part.
// Maybe even several.
// <[]===[]> or an ascii drawing of some sort.
/* If a lot is being said, don't be afraid to paste an entire article-length comment in here that describes anything. Also, these things should appear above functions and methods, but not in places where they aren't needed, unless you are using a documentor tool or have some other reason. */
```
* I use exactly one space instead of a tab, I deplore tabs and I delete them as much as possible and replace them with a single space. The code appears tight and it is easy to see what lines up, and it is the same on all editors on any computer. This rule eliminates the 5 versus 8 versus 10 tab spacing nonsense that has ruined the formatting of code since the beginning of ASCII. Programmers gain valuable visual cues when dealing with code that is precisely formatted.
* For those long spaghetti-like function calls, I use an indention as though I had entered a {,} convention. This groups the parameters, one or two to a line, and creates a nice block. It's easy to read and I like it to be readible on a single regular-sized code window. This is similar to the formatting desired in lengthy if () statements.
* I'm willing to change my rules for certain situations, but I am consistent throughout the entire project. I put this here at this part of the list because it's not the best thing to do often, so I often veto decisions to break from already established conventions or rules listed here.
* My if statements' first line ends with a ``{``
* I make my editor colors smart so that they provide psychological umpf. If it's a bad brace it should be outlined in red. If it's a good brace it should be green. If it's a reserved or POD-type, I want that to stand out because I often find I need to double check that I've picked the right one for casting reasons. I like -> and +,-%/ all to be less bright than numbers, but an eye-catching color like a blood-red so that your mind watches out for these minor thorns (danger!) .... I like numbers to be bright and a color that is very different than anything near them, they need a distinct color that is not used elsewhere. Currently, mine are bright green. Before, they were hot pink or bright orange, when the text around them wasn't nearly so flashy.
* I use the grid selection, cut and insert tools all the time. They were in "qedit" for DOS, Notepad++ and now thanks to a suggestion I made they are in MS Visual Studio. This helps greatly when inserting data into code in the form of default values, I like to align code separators like = ( " ", , ) so that I can use this feature.
* I use camelCase to differentiate variables of similar names that need to be at the same level of code. I also use camelCase to create more descriptive variable names like lightSurfaceIndicator or craftedMapItemContainer. If things get too cluttered, I'm not afraid to group them in a struct, but I prefer to do that when I've got a functional reason for them to be grouped. Once I do that, I usually want to expound upon that new struct into some mature functionality as soon as possible.
* When naming a class that is to be grouped, choose a prefix and don't be afraid to use the underscore (_) sparingly. If you are creating a global boolean, and it is a hacky switch, there is probably a better way to handle that, but if you absolutely must use one, make the_switch_an_entire_sentence_long because you made it global and it will be hard to locate.
* Keep variable names short when they are common among a majority of the classes. Abbreviation, however, is like obfuscation and when you are writing languages that compile to binary, you don't have to obfuscate. Common letters I use all follow a similar pattern and I've noticed when you break from that pattern, confusion sets in.
* A,B,C,D when dealing with quads or points of a triangle. In the case of a 14-sided figure I would use as many of the first letters of the alphabet as possible.
* L# this pattern (letter, number) when dealing with two identical figures, such as two triangles, where the convention would be A1,B1,C1,A2,B2,C2.
* I like to use lower case in the first pass of naming, and upper case when I run out of letters.
* I spell out Greek letters of mathematic significance (delta, theta)
* Unless there is a specific aesthetic reason, I prepend an underscore to enumerated keywords, or I use an appropriate prefix.
* I use capitalized concatenations of multiple words to describe related classes. Root classes fall at the end. This is the "backward" of the earlier rule regarding prefixing within a class group. An example of this is, say a root class is called Shader, then all of the individual implementations of shaders would have that as a post-fix. ie: GoochShader, LuxShader ; one thing to note about this rule is that sometimes other names collude with this.
* If x is int, xf is float and xd is double
* If only one color, it's color, if multiple, they are named appropriate.
* If a variable exists only for a temporary calculation or use, I name it after that intended use.
* I keep Macro Definitions to a minimum except for constant values and common reusable equations, but I'm careful and I read Bjarne Stroustrup's tips on C++ and you should too. * I've made a link to them at the bottom of this page for reference.
* I see conventions like these as a learning process, and I love enhancing my already robust set of practices. I absolutely adore "killing two birds with one stone"-type solutions.
* String functions get their own naming conventions that are named very differently than C, most of the time.
* I am not a big fan of namespaces, even though they feel safe. I just don't like them. I know how to identify when my stuff impacts the global namespace, but I work primarily within that namespace and I lament the words that are stolen by other developers. This might cause stranges issues with compiling though, so it is a bad habit but it is one that * I allow myself because I've broken many others.
* I don't prefer cout to older stdarg output (standard lib).
* I'm precise about plurality and what it means. Lists are plural, items singular.
* I do not use half-words like accel or acc very often because they are confusing.
* I like to simplify words like lightPosition to light, but with this particular one I've found that confusing.
"total" and "count" and "length" usually are similar types of values, but I avoid length because it is a common function name
* I rarely name a method count, I'd rather name it Count()
* I like to avoid constructors and keep them simple if I need them, and I'm never afraid of virtual methods as long as they are named properly.
* Specialized things need specialized names.
* Common letter abbreviations for variables I use over and over in graphics:
```
w = width, h = height, w2= w/2, h2= h/2
x,y,z, or a,b,c for points, vectors, etc
```
* Throwaway iterators most commonly occur in for() declarations, and when I use them it starts with 'i' since x is reserved for vectors. Subsequent ones follow the alphabet as j,k, but when I get to 'l', since 'l' looks like a 1, I jump to a different set such as m,n or p,q,r
* ``const char *p`` and related ``const char *q`` often, sometimes ``const char *r``
* I like function parameters to have verbose names, sometimes.
* f usually as a post-fix means "float", d means "double" and i means "int", u is unsigned, Z means "zeroed"
* d is short for delta usually
* t is short for time usually, sometimes theta
* v can mean velocity, but I usually write it out
* a can mean acceleration, but I usually write it out
* in for() loops variables are often borrowed from their type names, even if they are just one letter like: for ( int i=0; i<total; i++ )
* I use a self-created shorthand notation for naming variables that are related to basic arithmetic.
* If: somevar= another + another - another * another, I use the following convention: m is multiply, d is divide, p for plus and m for minus. I generally only use this when a variable has a single character for a name. An example is: int xp2m1=x+2-1;
* I admit variables like this have confusing names, especially with respect to 'divided by 2' and 'squared' versus 'multiplied by 2', where things become confusing. When I see something named this way I often look up what it equated to as reference during development.
* _ws can mean a post-fix indicating worldspace, there is also _ts for tangent space, _ns for normal space, _cs for camera space, _es which is eye space, etc., ect.
