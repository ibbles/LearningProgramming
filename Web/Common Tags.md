# Lists

An unordered list is created with the `ul` tag.
List items are created with the `li` tag.
Each item is prefixed with a circle.
```html
<ul>
	<li>An item</li>
	<li>Another item</li>
	<li>Yet another item</li>
</ul>
```

An ordered list is created with the `ol` tag.
List items are created with the `li` tag.
Each list item is prefixed with an increasing number.
```html
<ol>
	<li>An item</li>
	<li>Another item</li>
	<li>Yet another item</li>
</ol>
```


# Links

Links are created with the `a` tag along with its `href` attribute.
```html
<a href="https://www.example.com">Go to example.com</a>.
```

A link can point to anything that has a URL, not just other HTML documents.

To open the link target in a new tab add two more attributes to the `a` opening tag:
- `target`: `_blank`
	- Open the target in a new tab.
- `rel`: `noreferrer`
	- Prevent information about the current page from being shared with the target page.

```html
<a href="https://www.example.com" target="_blank" rel="noreferrer">Go to example.com</a>.
```

A link can be either relative or absolute.
Relative links are relative to the current document, not the site root.
Relative links should, in most cases, start with `./`.
(
When should we not prefix with `./`?
When are we required to prefix with `./`?
)


# Images

The `img` tag is a void tag.
The `src` attribute is used to identify the URL of the image to display.
The `src` attribute can be either relative or absolute.
An image should have an `alt` attribute describing the image.
An image should have the `width` and `height` attributes.
The `width` and `height` attributes is the image size in pixels, you should not include a `px` suffix on the size.

# References

- 1: [_The Odin Project_ > _Lists_ @ theodinproject.com](https://www.theodinproject.com/lessons/foundations-lists)
- 2: [_The Odin Project_ > _Links and Images_ @ theodinproject.com](https://www.theodinproject.com/lessons/foundations-links-and-images)
