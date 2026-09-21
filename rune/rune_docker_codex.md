This is a trial of using Codex CLI from a terminal within Rune.
I'm using `~/test` as the workspace: `:workspaceopen ~/test`.
Open a terminal: `<ctrl-meta-return>`.
The window now looks like this:
![[Pasted image 20260921202303.png]]
I have a script that creates a Docker container with the current directory mounted from a Docker image with Codex CLI installed.
Here is the welcome screen after running `codex` in the Docker container:
![[Pasted image 20260921202854.png]]

The menu looks a little... off.
This is version 0.153.4, not 10-something.

For comparison, here is what it looks like in Alacritty:
![[Pasted image 20260921205714.png]]

I selected skip and got the prompt input.
I start typing and a few characters in, after having typed `This is ` , I hit `<esc>` for whatever reason:
![[Pasted image 20260921210028.png]]

Not sure why hitting `<esc>` would try to open an external editor. Hitting `<esc>` at the sames state in Alacritty has no visible effect.
Not sure what to do about the `Save and close external editor to continue.` message, there is no external editor open.

I want to get my prompt back somehow, so I can keep typing my message.
Hitting `<esc>` again to try and reset the state doesn't help much:
![[Pasted image 20260921210305.png]]

Now the new characters I had written have been overwritten by the error messages.
I suspect the prompt is still in memory though, I just need to get it back.

Let's try the arrow keys, walking right as far as the right arrow key will let me:
![[Pasted image 20260921210548.png]]

That's about the length of `This is `, the prompt I have written so far.

Sometimes a `<ctrl-l>` will redraw the screen.
It does!
I get this:
![[Pasted image 20260921210703.png]]

Looking good again.

Next I want to paste something from the system clipboard.
In a regular file editor I can paste by making sure I'm in normal mode and then hit `<p>`.
Not sure how to enter normal mode here, since `<esc>` didn't.
I also can't open the command prompt since one must be in normal mode to do that, typing `:` will send a `:` to Codex, not open the command prompt.

It is common among other terminal emulators that `<ctrl-shift-v>` pastes.
Here it instead produces this:
![[Pasted image 20260921211337.png]]
I'm on a regular local Xorg session, no remote desktop of SSH involved.
I can add there is no image in the clipboard, just regular text: `ot in a session directo`.
Now the terminal is messed up again, so reset with `<ctrl-l>`.

Middle-click is also a common  way to paste stuff.
It has a different buffer though, so let's click-and-drag select some text in Firefox, which will put the selected text into the selection buffer.
Here is a middle-click: `I can provide a more concrete description of what is happening when I'm back home again.`.
And into Codex-in-Docker-in-Rune:
![[Pasted image 20260921211719.png]]
It did paste, but not the text I expected, it pasted the text I had in the `<ctrl-c>` copy buffer.
Good to know, I didn't think it would do that.

Let's send a prompt.
![[Pasted image 20260921212155.png]]

At first glance the output look alright, but the output is actually a bit longer than what is visible, it is not possible to scroll more than a single line, and the orange numbers mixed in with the last row is possibly remnants of the Codex status line where the 8 could be 8% context used but I'm not sure what the 34 might be so it could be just garbage characters.

For comparison, in Alacritty I get properly rendered output, I can scroll through the entire response, and the status line stay complete as the last line:
![[Pasted image 20260921212614.png]]

Curiously, after splitting the Rune window with `<ctrl-meta-n>` I get a half-window that is more usable.
The entire reply is readable, I can scroll through the conversation, and the status line is rendered properly.

![[Pasted image 20260921212908.png]]

The citations look a bit weird, not sure what's up with that.

That's about all experimentation I have time for at this time.
Let's summarize:
- Don't hit Esc.
- I was not able to figure out how to open the Rune command prompt.
- The terminal get messed up from time to time, redraw the terminal with `<ctrl-l>`.
- Splitting the window caused the Codex output to render better.
	- It is possible that a window resize, or a redraw with `<ctrl-l>`, would have the same effect.
- Use middle-click to paste.
	- Will paste from the system `<ctrl-c>` buffer, not the system click-and-drag buffer.
- Click-and-drag to copy text.
	- I noticed that a new copy event is created for every character, which quickly takes over my clipboard history.
	- Not sure if the copying is done by Rune or Codex, click-and-drag in a regular file buffer window does not copy anything, but neither does click-and-drag in Codex-in-Alacritty.
![[Pasted image 20260921213504.png]]

