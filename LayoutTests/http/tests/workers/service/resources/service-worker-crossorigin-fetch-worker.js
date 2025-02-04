var status = "no status";
self.addEventListener("fetch", (event) => {
    if (event.request.url.indexOf("status") !== -1) {
        event.respondWith(new Response(null, {status: 200, statusText: status}));
        return;
    }
    if (!event.request.url.endsWith(".fromserviceworker")) {
        state = "unknown url";
        event.respondWith(new Response(null, {status: 404, statusText: "Not Found"}));
        return;
    }
    // Changing cors fetch into same origin fetch.
    status = event.request.url.substring(21, event.request.url.length - 18) + " through " + "fetch";
    event.respondWith(fetch(event.request.url.substring(21, event.request.url.length - 18)));
});
